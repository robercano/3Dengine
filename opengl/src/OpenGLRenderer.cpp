/**
 * @class	OpenGLRenderer
 * @brief	OpenGL based 3D renderer
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include "OpenGL.h"
#include <algorithm>
#include "Asset3DStorage.hpp"
#include "Logging.hpp"
#include "OpenGLAsset3D.hpp"
#include "OpenGLAsset3D.hpp"
#include "OpenGLLightingShader.hpp"
#include "OpenGLRenderer.hpp"

using namespace Logging;

bool OpenGLRenderer::init()
{
    std::string error;

    __(glClearColor(0.0, 0.0, 0.0, 1.0));
    __(glEnable(GL_DEPTH_TEST));
    __(glEnable(GL_CULL_FACE));
    __(glEnable(GL_BLEND));
    __(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    /* There seems to be a bug with sRGB default framebuffer, as
     * the buffer is always linear, but if we enable this the
     * drivers are doing the linear->sRGB conversion anyway!
     * So don't enable it for now, we will have to compensate
     * in the shaders */
    //__( glEnable(GL_FRAMEBUFFER_SRGB) );
    __(glCullFace(GL_BACK));
    __(glDisable(GL_DITHER));
    __(glDisable(GL_LINE_SMOOTH));
    __(glDisable(GL_POLYGON_SMOOTH));
    __(glHint(GL_POLYGON_SMOOTH_HINT, GL_DONT_CARE));
#define GL_MULTISAMPLE_ARB 0x809D
    __(glDisable(GL_MULTISAMPLE_ARB));
    __(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
    __(glDepthRangef(0.1f, 1000.0f));

    /* Generate a fake texture to fullfill GLSL 3.3 requirement
     * on some cards that need all samplers to be bound to a valid
     * texture, even if they are not used */
    __(glGenTextures(1, &_dummyTexture));

    /* Also generate a texture for no-shadow receivers that is
     * completely white */
    __(glGenTextures(1, &_noshadowTexture));
    __(glBindTexture(GL_TEXTURE_2D, _noshadowTexture));
    {
        unsigned char noShadow[3] = {255, 255, 255};

        __(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
        __(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, noShadow));
        __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
        __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    }
    __(glBindTexture(GL_TEXTURE_2D, 0));

    /* TODO: Once we use our own format, this should not be needed */
    __(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
    __(glBindTexture(GL_TEXTURE_2D, _dummyTexture));
    {
        __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    }
    __(glBindTexture(GL_TEXTURE_2D, 0));

    /* Create the shader to render light debug billboard */
    if (_renderLightShader.use("utils/render_light", error) != true) {
        log("ERROR loading utils/render_light shader: %s\n", error.c_str());
        return false;
    }

    /* Create the shader to render bounding boxes information */
    if (_renderBoundingBox.use("utils/render_boundingbox", error) != true) {
        log("ERROR loading utils/render_boundingbox shader: %s\n", error.c_str());
        return false;
    }

    /* Create the shader to render bounding spheres information */
    if (_renderBoundingSphere.use("utils/render_boundingsphere", error) != true) {
        log("ERROR loading utils/render_boundingsphere shader: %s\n", error.c_str());
        return false;
    }

    if (_renderNormals.use("utils/render_normals", error) != true) {
        log("ERROR loading utils/render_normals shader: %s\n", error.c_str());
        return false;
    }

    /* Call parent to initialize some members related to scene rendering */
    return Renderer::init();
}

const char *OpenGLRenderer::getName() { return (const char *)glGetString(GL_RENDERER); }
const char *OpenGLRenderer::getVersion() { return (const char *)glGetString(GL_VERSION); }
const char *OpenGLRenderer::getVendor() { return (const char *)glGetString(GL_VENDOR); }
const char *OpenGLRenderer::getShaderVersion() { return (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION); }
Shader *OpenGLRenderer::newShader(void) { return new OpenGLShader(); }
Asset3D *OpenGLRenderer::loadAsset3D(const std::string &assetName)
{
    OpenGLAsset3D *asset = new OpenGLAsset3D();
    if (asset == NULL) {
        log("ERROR allocating memory for OpenGLAsset3D\n");
        return NULL;
    }

    if (Asset3DStorage::Load(assetName, *asset) == false) {
        log("ERROR loading asset %s into an OpenGLAsset3D\n", assetName.c_str());
        delete asset;
        return NULL;
    }

    if (asset->prepare() == false) {
        log("ERROR preparing OpenGL arrays for asset %s\n", assetName.c_str());
        delete asset;
        return NULL;
    }

    return asset;
}

bool OpenGLRenderer::prepareAsset3D(Asset3D &source)
{
    OpenGLAsset3D &glAsset3D = static_cast<OpenGLAsset3D &>(source);
    if (glAsset3D.prepare() == false) {
        log("ERROR preparing OpenGL Asset3D\n");
        return false;
    }

    return true;
}

bool OpenGLRenderer::renderModel3D(Model3D &model3D, Camera &camera, LightingShader &shader, DirectLight *sun,
                                   std::vector<PointLight *> &pointLights, std::vector<SpotLight *> &spotLights, float ambientK,
                                   RenderTarget &renderTarget, bool disableDepth)
{
    glm::mat4 biasMatrix(0.5, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.5, 0.5, 0.5, 1.0);
    GLuint textureUnit = 0;
    GLuint dummyTextureUnit = 0;

    __(glDepthRangef(camera.getNear(), camera.getFar()));

    /* Calculate MVP matrix */
    glm::mat4 MVP = camera.getPerspectiveMatrix() * camera.getViewMatrix() * model3D.getModelMatrix();

    /* Calculate normal matrix */
    glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model3D.getModelMatrix())));

    /* Cast the model into an internal type */
    OpenGLAsset3D *glObject = static_cast<OpenGLAsset3D *>(model3D.getAsset3D());

    if (disableDepth) {
        glDisable(GL_DEPTH_TEST);
    } else {
        glEnable(GL_DEPTH_TEST);
    }

    if (getWireframeMode() == true) {
        __(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
        __(glEnable(GL_LINE_SMOOTH));
        __(glDisable(GL_CULL_FACE));
    } else {
        __(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
        __(glDisable(GL_LINE_SMOOTH));
        __(glEnable(GL_CULL_FACE));
    }

    /* Bind the render target */
    renderTarget.bind();
    {
        __(glEnable(GL_MULTISAMPLE));

        __(glEnable(GL_DEPTH_TEST));
        __(glBlendEquation(GL_FUNC_ADD));
        __(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        __(glEnable(GL_BLEND));

        /* Bind program to upload the uniform */
        shader.attach();

        /* Send our transformation to the currently bound shader, in the "MVP" uniform */
        shader.setUniformMat4("u_MVPMatrix", &MVP);
        shader.setUniformMat4("u_viewMatrix", &camera.getViewMatrix());
        shader.setUniformMat4("u_modelMatrix", &model3D.getModelMatrix());
        shader.setUniformMat3("u_normalMatrix", &normalMatrix);
        shader.setUniformTexture2D("u_diffuseMap", textureUnit++);
        shader.setUniformFloat("u_ambientK", ambientK);

        /* Activate and bind unit 0 for the dummy texture */
        dummyTextureUnit = textureUnit++;
        __(glActiveTexture(GL_TEXTURE0 + dummyTextureUnit));
        __(glBindTexture(GL_TEXTURE_2D, _dummyTexture));

        /* Set the sun light */
        if (sun != NULL) {
            glm::mat4 shadowMVP = sun->getProjectionMatrix() * sun->getViewMatrix() * model3D.getModelMatrix();
            shadowMVP = biasMatrix * shadowMVP;

            shader.setDirectLight(*sun);
            shader.setUniformUint("u_numDirectLights", 1);

            /* TODO: This has to be set in a matrix array */
            shader.setUniformMat4("u_shadowMVPDirectLight", &shadowMVP);
            shader.setUniformTexture2D("u_shadowMapDirectLight", textureUnit);

            __(glActiveTexture(GL_TEXTURE0 + textureUnit));
            if (model3D.isShadowReceiver()) {
                sun->getShadowMap()->bindDepth();
            } else {
                __(glBindTexture(GL_TEXTURE_2D, _noshadowTexture));
            }

            textureUnit++;
        } else {
            shader.setUniformUint("u_numDirectLights", 0);
            shader.setUniformTexture2D("u_shadowMapDirectLight", dummyTextureUnit);
        }

        /* Point lights */
        glm::mat4 *shadowMVPArray = new glm::mat4[pointLights.size()];
        GLuint texturesArray[OpenGLLightingShader::MAX_LIGHTS];

        for (uint32_t numLight = 0; numLight < pointLights.size(); ++numLight) {
            shader.setPointLight(numLight, *pointLights[numLight]);

            /* Calculate adjusted shadow map matrix */
            glm::mat4 shadowMVP =
                pointLights[numLight]->getProjectionMatrix() * pointLights[numLight]->getViewMatrix() * model3D.getModelMatrix();

            shadowMVPArray[numLight] = biasMatrix * shadowMVP;
            texturesArray[numLight] = textureUnit;

            __(glActiveTexture(GL_TEXTURE0 + textureUnit));
            if (model3D.isShadowReceiver()) {
                pointLights[numLight]->getShadowMap()->bindDepth();
            } else {
                __(glBindTexture(GL_TEXTURE_2D, _noshadowTexture));
            }

            textureUnit++;
        }
        for (uint32_t numLight = pointLights.size(); numLight < OpenGLLightingShader::MAX_LIGHTS; ++numLight) {
            texturesArray[numLight] = dummyTextureUnit;
        }

        shader.setUniformMat4("u_shadowMVPPointLight[0]", shadowMVPArray, pointLights.size());
        shader.setUniformTexture2DArray("u_shadowMapPointLight[0]", texturesArray, OpenGLLightingShader::MAX_LIGHTS);
        shader.setUniformUint("u_numPointLights", pointLights.size());

        /* Free the resources */
        delete[] shadowMVPArray;

        /* Spotlights */
        shadowMVPArray = new glm::mat4[spotLights.size()];

        for (uint32_t numLight = 0; numLight < spotLights.size(); ++numLight) {
            shader.setSpotLight(numLight, *spotLights[numLight]);

            /* Calculate adjusted shadow map matrix */
            glm::mat4 shadowMVP =
                spotLights[numLight]->getProjectionMatrix() * spotLights[numLight]->getViewMatrix() * model3D.getModelMatrix();

            shadowMVPArray[numLight] = biasMatrix * shadowMVP;
            texturesArray[numLight] = textureUnit;

            __(glActiveTexture(GL_TEXTURE0 + textureUnit));
            if (model3D.isShadowReceiver()) {
                spotLights[numLight]->getShadowMap()->bindDepth();
            } else {
                __(glBindTexture(GL_TEXTURE_2D, _noshadowTexture));
            }

            textureUnit++;
        }
        for (uint32_t numLight = spotLights.size(); numLight < OpenGLLightingShader::MAX_LIGHTS; ++numLight) {
            texturesArray[numLight] = dummyTextureUnit;
        }

        shader.setUniformMat4("u_shadowMVPSpotLight[0]", shadowMVPArray, spotLights.size());
        shader.setUniformTexture2DArray("u_shadowMapSpotLight[0]", texturesArray, OpenGLLightingShader::MAX_LIGHTS);
        shader.setUniformUint("u_numSpotLights", spotLights.size());

        /* Free the resources */
        delete[] shadowMVPArray;

        /* Draw the model */
        __(glBindVertexArray(glObject->getVertexArrayID()));
        {
            __(glActiveTexture(GL_TEXTURE0));

            std::vector<Material> materials = glObject->getMaterials();
            std::vector<uint32_t> texturesIDs = glObject->getTexturesIDs();
            std::vector<uint32_t> offset = glObject->getIndicesOffsets();
            std::vector<uint32_t> count = glObject->getIndicesCount();

            for (size_t i = 0; i < materials.size(); ++i) {
                __(glBindTexture(GL_TEXTURE_2D, texturesIDs[i]));
                shader.setMaterial(materials[i]);

                __(glDrawElements(GL_TRIANGLES, count[i], GL_UNSIGNED_INT, (void *)(offset[i] * sizeof(GLuint))));
            }
        }
        __(glBindVertexArray(0));

        /* Set the shader custom parameters */
        shader.setCustomParams();

        /* Unbind */
        shader.detach();
    }
    renderTarget.unbind();

    return true;
}
bool OpenGLRenderer::renderModel3D(Model3D &model3D, Camera &camera, LightingShader &shader, DirectLight *sun,
                                   std::vector<PointLight *> &pointLights, std::vector<SpotLight *> &spotLights, float ambientK,
                                   RenderTarget &renderTarget, bool disableDepth)
{
    glm::mat4 biasMatrix(0.5, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.5, 0.5, 0.5, 1.0);
    GLuint textureUnit = 0;
    GLuint dummyTextureUnit = 0;

    __(glDepthRangef(camera.getNear(), camera.getFar()));

    /* Calculate MVP matrix */
    glm::mat4 MVP = camera.getPerspectiveMatrix() * camera.getViewMatrix() * model3D.getModelMatrix();

    /* Calculate normal matrix */
    glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model3D.getModelMatrix())));

    /* Cast the model into an internal type */
    OpenGLAsset3D *glObject = static_cast<OpenGLAsset3D *>(model3D.getAsset3D());

    if (disableDepth) {
        glDisable(GL_DEPTH_TEST);
    } else {
        glEnable(GL_DEPTH_TEST);
    }

    if (getWireframeMode() == true) {
        __(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
        __(glEnable(GL_LINE_SMOOTH));
        __(glDisable(GL_CULL_FACE));
    } else {
        __(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
        __(glDisable(GL_LINE_SMOOTH));
        __(glEnable(GL_CULL_FACE));
    }

    /* Bind the render target */
    renderTarget.bind();
    {
        __(glEnable(GL_MULTISAMPLE));

        __(glEnable(GL_DEPTH_TEST));
        __(glBlendEquation(GL_FUNC_ADD));
        __(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        __(glEnable(GL_BLEND));

        /* Bind program to upload the uniform */
        shader.attach();

        /* Send our transformation to the currently bound shader, in the "MVP" uniform */
        shader.setUniformMat4("u_MVPMatrix", &MVP);
        shader.setUniformMat4("u_viewMatrix", &camera.getViewMatrix());
        shader.setUniformMat4("u_modelMatrix", &model3D.getModelMatrix());
        shader.setUniformMat3("u_normalMatrix", &normalMatrix);
        shader.setUniformTexture2D("u_diffuseMap", textureUnit++);
        shader.setUniformFloat("u_ambientK", ambientK);

        /* Activate and bind unit 0 for the dummy texture */
        dummyTextureUnit = textureUnit++;
        __(glActiveTexture(GL_TEXTURE0 + dummyTextureUnit));
        __(glBindTexture(GL_TEXTURE_2D, _dummyTexture));

        /* Set the sun light */
        if (sun != NULL) {
            glm::mat4 shadowMVP = sun->getProjectionMatrix() * sun->getViewMatrix() * model3D.getModelMatrix();
            shadowMVP = biasMatrix * shadowMVP;

            shader.setDirectLight(*sun);
            shader.setUniformUint("u_numDirectLights", 1);

            /* TODO: This has to be set in a matrix array */
            shader.setUniformMat4("u_shadowMVPDirectLight", &shadowMVP);
            shader.setUniformTexture2D("u_shadowMapDirectLight", textureUnit);

            __(glActiveTexture(GL_TEXTURE0 + textureUnit));
            if (model3D.isShadowReceiver()) {
                sun->getShadowMap()->bindDepth();
            } else {
                __(glBindTexture(GL_TEXTURE_2D, _noshadowTexture));
            }

            textureUnit++;
        } else {
            shader.setUniformUint("u_numDirectLights", 0);
            shader.setUniformTexture2D("u_shadowMapDirectLight", dummyTextureUnit);
        }

        /* Point lights */
        glm::mat4 *shadowMVPArray = new glm::mat4[pointLights.size()];
        GLuint texturesArray[OpenGLLightingShader::MAX_LIGHTS];

        for (uint32_t numLight = 0; numLight < pointLights.size(); ++numLight) {
            shader.setPointLight(numLight, *pointLights[numLight]);

            /* Calculate adjusted shadow map matrix */
            glm::mat4 shadowMVP =
                pointLights[numLight]->getProjectionMatrix() * pointLights[numLight]->getViewMatrix() * model3D.getModelMatrix();

            shadowMVPArray[numLight] = biasMatrix * shadowMVP;
            texturesArray[numLight] = textureUnit;

            __(glActiveTexture(GL_TEXTURE0 + textureUnit));
            if (model3D.isShadowReceiver()) {
                pointLights[numLight]->getShadowMap()->bindDepth();
            } else {
                __(glBindTexture(GL_TEXTURE_2D, _noshadowTexture));
            }

            textureUnit++;
        }
        for (uint32_t numLight = pointLights.size(); numLight < OpenGLLightingShader::MAX_LIGHTS; ++numLight) {
            texturesArray[numLight] = dummyTextureUnit;
        }

        shader.setUniformMat4("u_shadowMVPPointLight[0]", shadowMVPArray, pointLights.size());
        shader.setUniformTexture2DArray("u_shadowMapPointLight[0]", texturesArray, OpenGLLightingShader::MAX_LIGHTS);
        shader.setUniformUint("u_numPointLights", pointLights.size());

        /* Free the resources */
        delete[] shadowMVPArray;

        /* Spotlights */
        shadowMVPArray = new glm::mat4[spotLights.size()];

        for (uint32_t numLight = 0; numLight < spotLights.size(); ++numLight) {
            shader.setSpotLight(numLight, *spotLights[numLight]);

            /* Calculate adjusted shadow map matrix */
            glm::mat4 shadowMVP =
                spotLights[numLight]->getProjectionMatrix() * spotLights[numLight]->getViewMatrix() * model3D.getModelMatrix();

            shadowMVPArray[numLight] = biasMatrix * shadowMVP;
            texturesArray[numLight] = textureUnit;

            __(glActiveTexture(GL_TEXTURE0 + textureUnit));
            if (model3D.isShadowReceiver()) {
                spotLights[numLight]->getShadowMap()->bindDepth();
            } else {
                __(glBindTexture(GL_TEXTURE_2D, _noshadowTexture));
            }

            textureUnit++;
        }
        for (uint32_t numLight = spotLights.size(); numLight < OpenGLLightingShader::MAX_LIGHTS; ++numLight) {
            texturesArray[numLight] = dummyTextureUnit;
        }

        shader.setUniformMat4("u_shadowMVPSpotLight[0]", shadowMVPArray, spotLights.size());
        shader.setUniformTexture2DArray("u_shadowMapSpotLight[0]", texturesArray, OpenGLLightingShader::MAX_LIGHTS);
        shader.setUniformUint("u_numSpotLights", spotLights.size());

        /* Free the resources */
        delete[] shadowMVPArray;

        /* Draw the model */
        __(glBindVertexArray(glObject->getVertexArrayID()));
        {
            __(glActiveTexture(GL_TEXTURE0));

            std::vector<Material> materials = glObject->getMaterials();
            std::vector<uint32_t> texturesIDs = glObject->getTexturesIDs();
            std::vector<uint32_t> offset = glObject->getIndicesOffsets();
            std::vector<uint32_t> count = glObject->getIndicesCount();

            for (size_t i = 0; i < materials.size(); ++i) {
                __(glBindTexture(GL_TEXTURE_2D, texturesIDs[i]));
                shader.setMaterial(materials[i]);

                __(glDrawElements(GL_TRIANGLES, count[i], GL_UNSIGNED_INT, (void *)(offset[i] * sizeof(GLuint))));
            }
        }
        __(glBindVertexArray(0));

        /* Set the shader custom parameters */
        shader.setCustomParams();

        /* Unbind */
        shader.detach();
    }
    renderTarget.unbind();

    return true;
}

bool OpenGLRenderer::renderToShadowMap(Model3D &model3D, Light &light, NormalShadowMapShader &shader)
{
    /* Calculate MVP matrix */
    glm::mat4 MVP = light.getProjectionMatrix() * light.getViewMatrix() * model3D.getModelMatrix();

    /* Calculate normal matrix */
    glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model3D.getModelMatrix())));

    /* Cast the model into an internal type */
    OpenGLAsset3D *glObject = static_cast<OpenGLAsset3D *>(model3D.getAsset3D());

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    /* Bind the render target */
    light.getShadowMap()->bind();
    {
        /* Bind program to upload the uniform */
        shader.attach();

        /* Send our transformation to the currently bound shader, in the "MVP" uniform */
        shader.setUniformMat4("u_MVPMatrix", &MVP);

        /* Draw the model */
        __(glBindVertexArray(glObject->getVertexArrayID()));
        {
            std::vector<uint32_t> offset = glObject->getIndicesOffsets();
            std::vector<uint32_t> count = glObject->getIndicesCount();

            for (size_t i = 0; i < count.size(); ++i) {
                __(glDrawElements(GL_TRIANGLES, count[i], GL_UNSIGNED_INT, (void *)(offset[i] * sizeof(GLuint))));
            }
        }
        __(glBindVertexArray(0));

        /* Unbind */
        shader.detach();
    }
    light.getShadowMap()->unbind();

    return true;
}

bool OpenGLRenderer::renderLight(Light &light, Camera &camera, RenderTarget &renderTarget, uint32_t lightNumber)
{
    glm::vec3 ambient = (light.getAmbient() + light.getDiffuse() + light.getSpecular()) / 3.0f;

    /* Calculate MVP matrix */
    glm::mat4 MV = camera.getViewMatrix() * light.getModelMatrix();
    glm::mat4 P = camera.getPerspectiveMatrix();

    /* Bind the render target */
    renderTarget.bind();
    {
        __(glEnable(GL_DEPTH_TEST));
        __(glBlendEquation(GL_FUNC_ADD));
        __(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        __(glEnable(GL_BLEND));
        __(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));

        GLuint lightPosVAO, lightPosVBO;

        /* Bind program to upload the uniform */
        _renderLightShader.attach();

        /* Send our transformation to the currently bound shader, in the "MVP" uniform */
        _renderLightShader.setUniformMat4("u_MVMatrix", &MV);
        _renderLightShader.setUniformMat4("u_PMatrix", &P);
        _renderLightShader.setUniformVec3("u_lightColor", ambient);
        _renderLightShader.setUniformUint("u_lightNumber", lightNumber);

        __(glGenVertexArrays(1, &lightPosVAO));
        __(glBindVertexArray(lightPosVAO));

        __(glGenBuffers(1, &lightPosVBO));

        __(glBindBuffer(GL_ARRAY_BUFFER, lightPosVBO));
        __(glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3), &light.getPosition()[0], GL_STATIC_DRAW));

        __(glEnable(GL_PROGRAM_POINT_SIZE));
        __(glDrawArrays(GL_POINTS, 0, 1));

        __(glDisable(GL_PROGRAM_POINT_SIZE));

        __(glBindVertexArray(0));

        __(glDeleteBuffers(1, &lightPosVBO));
        __(glDeleteVertexArrays(1, &lightPosVAO));

        /* Unbind */
        _renderLightShader.detach();
    }
    renderTarget.unbind();

    return true;
}

bool OpenGLRenderer::renderLights(std::vector<Light *> &lights, Camera &camera, RenderTarget &renderTarget)
{
    struct light_compare {
        light_compare(Camera &c) : _camera(c) {}
        inline bool operator()(const Light *light1, const Light *light2)
        {
            return glm::length(_camera.getPosition() - light1->getPosition()) > glm::length(_camera.getPosition() - light2->getPosition());
        }
        Camera &_camera;
    };

    /* Sort the lights by its inverse proximity to the camera */
    std::sort(lights.begin(), lights.end(), light_compare(camera));

    uint32_t n = 0;
    for (std::vector<Light *>::iterator it = lights.begin(); it != lights.end(); ++it, ++n) {
        if (renderLight(*(*it), camera, renderTarget, n) == false) {
            return false;
        }
    }
    return true;
}

bool OpenGLRenderer::renderBoundingBox(const BoundingBox &box, const glm::mat4 &modelMatrix, const glm::vec3 &color, Camera &camera,
                                       RenderTarget &renderTarget)
{
    /* Generate the box lines */
    GLfloat boxFaces[] = {
        /* First face */
        box.getMin().x, box.getMin().y, box.getMin().z, box.getMax().x, box.getMin().y, box.getMin().z, box.getMin().x, box.getMin().y,
        box.getMin().z, box.getMin().x, box.getMax().y, box.getMin().z, box.getMin().x, box.getMin().y, box.getMin().z, box.getMin().x,
        box.getMin().y, box.getMax().z,
        /*--*/
        box.getMax().x, box.getMax().y, box.getMax().z, box.getMin().x, box.getMax().y, box.getMax().z, box.getMax().x, box.getMax().y,
        box.getMax().z, box.getMax().x, box.getMin().y, box.getMax().z, box.getMax().x, box.getMax().y, box.getMax().z, box.getMax().x,
        box.getMax().y, box.getMin().z,
        /*--*/
        box.getMin().x, box.getMax().y, box.getMax().z, box.getMin().x, box.getMax().y, box.getMin().z, box.getMin().x, box.getMax().y,
        box.getMax().z, box.getMin().x, box.getMin().y, box.getMax().z,
        /*--*/
        box.getMax().x, box.getMin().y, box.getMax().z, box.getMax().x, box.getMin().y, box.getMin().z, box.getMax().x, box.getMin().y,
        box.getMax().z, box.getMin().x, box.getMin().y, box.getMax().z,
        /*--*/
        box.getMax().x, box.getMax().y, box.getMin().z, box.getMin().x, box.getMax().y, box.getMin().z, box.getMax().x, box.getMax().y,
        box.getMin().z, box.getMax().x, box.getMin().y, box.getMin().z,
    };

    /* Calculate MVP matrix, bounding box coordinates are already in world coordinates */
    glm::mat4 MVP = camera.getPerspectiveMatrix() * camera.getViewMatrix() * modelMatrix;

    __(glEnable(GL_DEPTH_TEST));

    /* Bind the render target */
    renderTarget.bind();
    {
        GLuint boxPosVAO, boxPosVBO;

        /* Bind program to upload the uniform */
        _renderBoundingBox.attach();

        /* Send our transformation to the currently bound _renderBoundingBox, in the "MVP" uniform */
        _renderBoundingBox.setUniformMat4("u_MVPMatrix", &MVP);
        _renderBoundingBox.setUniformVec3("u_boxColor", const_cast<glm::vec3 &>(color));

        __(glGenVertexArrays(1, &boxPosVAO));
        __(glBindVertexArray(boxPosVAO));

        __(glGenBuffers(1, &boxPosVBO));

        __(glBindBuffer(GL_ARRAY_BUFFER, boxPosVBO));

        __(glBufferData(GL_ARRAY_BUFFER, sizeof boxFaces, boxFaces, GL_STATIC_DRAW));

        __(glEnableVertexAttribArray(0));
        __(glVertexAttribPointer(0,         // attribute. No particular reason for 0, but must match the layout in the shader.
                                 3,         // size
                                 GL_FLOAT,  // type
                                 GL_FALSE,  // normalized?
                                 0,         // stride
                                 (void *)0  // array buffer offset
                                 ));

        __(glDrawArrays(GL_LINES, 0, sizeof boxFaces / (2 * sizeof *boxFaces)));

        __(glBindVertexArray(0));

        __(glDeleteBuffers(1, &boxPosVBO));
        __(glDeleteVertexArrays(1, &boxPosVAO));

        /* Unbind */
        _renderBoundingBox.detach();
    }
    renderTarget.unbind();

    return true;
}

bool OpenGLRenderer::renderBoundingSphere(const BoundingSphere &sphere, const glm::vec3 &center, const glm::vec3 &color, Camera &camera,
                                          RenderTarget &renderTarget)
{
    /* Calculate MVP matrix, bounding box coordinates are already in world coordinates */
    glm::mat4 MVP = camera.getPerspectiveMatrix() * camera.getViewMatrix();

    /* Bind the render target */
    renderTarget.bind();
    {
        GLuint boxPosVAO, boxPosVBO;

        __(glEnable(GL_DEPTH_TEST));

        //__(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
        //__(glEnable(GL_CULL_FACE));

        /* Bind program to upload the uniform */
        _renderBoundingSphere.attach();

        /* Send our transformation to the currently bound _renderBoundingSpheres, in the "MVP" uniform */
        _renderBoundingSphere.setUniformMat4("u_MVPMatrix", &MVP);
        _renderBoundingSphere.setUniformMat4("u_projectionMatrix", &camera.getPerspectiveMatrix());
        _renderBoundingSphere.setUniformVec3("u_boxColor", const_cast<glm::vec3 &>(color));
        _renderBoundingSphere.setUniformFloat("u_radius", sphere.getRadius());

        __(glGenVertexArrays(1, &boxPosVAO));
        __(glBindVertexArray(boxPosVAO));

        __(glGenBuffers(1, &boxPosVBO));

        __(glBindBuffer(GL_ARRAY_BUFFER, boxPosVBO));

        __(glBufferData(GL_ARRAY_BUFFER, sizeof center, &center[0], GL_STATIC_DRAW));

        __(glEnableVertexAttribArray(0));
        __(glVertexAttribPointer(0,  // attribute. No particular reason for 0, but must match the layout in the _renderBoundingSpheres.
                                 3,  // size
                                 GL_FLOAT,  // type
                                 GL_FALSE,  // normalized?
                                 0,         // stride
                                 (void *)0  // array buffer offset
                                 ));

        __(glDrawArrays(GL_POINTS, 0, 1));

        __(glBindVertexArray(0));

        __(glDeleteBuffers(1, &boxPosVBO));
        __(glDeleteVertexArrays(1, &boxPosVAO));

        /* Unbind */
        _renderBoundingSphere.detach();
    }
    renderTarget.unbind();

    return true;
}

bool OpenGLRenderer::renderBoundingVolumes(Object3D &object, Camera &camera, RenderTarget &renderTarget, bool showSphere, bool showAABB,
                                           bool showOOBB)
{
    if (showSphere) {
        if (renderBoundingSphere(object.getBoundingSphere(), object.getPosition(), glm::vec3(1.0f, 0.0f, 0.0f), camera, renderTarget) ==
            false) {
            return false;
        }
    }
    if (showAABB) {
        if (renderBoundingBox(object.getAABB(), glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f), camera, renderTarget) == false) {
            return false;
        }
    }
    if (showOOBB) {
        if (renderBoundingBox(object.getOOBB(), object.getModelMatrix(), glm::vec3(0.0f, 0.0f, 1.0f), camera, renderTarget) == false) {
            return false;
        }
    }
    return true;
}

bool OpenGLRenderer::renderModelNormals(Model3D &model3D, Camera &camera, RenderTarget &renderTarget, float normalSize)
{
    /* Calculate MVP matrix */
    glm::mat4 MVP = camera.getPerspectiveMatrix() * camera.getViewMatrix() * model3D.getModelMatrix();

    /* Calculate normal matrix */
    glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model3D.getModelMatrix())));

    /* Cast the model into an internal type */
    OpenGLAsset3D *glObject = static_cast<OpenGLAsset3D *>(model3D.getAsset3D());

    /* Bind the render target */
    renderTarget.bind();
    {
        /* Bind program to upload the uniform */
        _renderNormals.attach();

        _renderNormals.setUniformMat4("u_MVPMatrix", &MVP);
        _renderNormals.setUniformFloat("u_normalSize", normalSize);

        /* Draw the model */
        __(glBindVertexArray(glObject->getVertexArrayID()));
        {
            std::vector<uint32_t> offset = glObject->getIndicesOffsets();
            std::vector<uint32_t> count = glObject->getIndicesCount();

            for (size_t i = 0; i < offset.size(); ++i) {
                __(glDrawElements(GL_TRIANGLES, count[i], GL_UNSIGNED_INT, (void *)(offset[i] * sizeof(GLuint))));
            }
        }
        __(glBindVertexArray(0));

        /* Unbind */
        _renderNormals.detach();
    }
    renderTarget.unbind();

    return true;
}

bool OpenGLRenderer::resize(uint16_t width, uint16_t height)
{
    _width = width;
    _height = height;
    return true;
}

void OpenGLRenderer::flush() { glFinish(); }
void OpenGLRenderer::clear()
{
    __(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    __(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    __(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}
