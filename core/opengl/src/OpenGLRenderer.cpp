/**
 * @class	OpenGLRenderer
 * @brief	OpenGL based 3D renderer
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include "OpenGL.h"
#include "OpenGLRenderer.hpp"
#include "OpenGLShader.hpp"
#include "OpenGLModel3D.hpp"

void OpenGLRenderer::init()
{
	GL( glClearColor(0.0, 0.0, 0.0, 1.0) );
	GL( glEnable(GL_DEPTH_TEST) );
	GL( glEnable(GL_CULL_FACE) );
	GL( glEnable(GL_BLEND) );
    GL( glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) );
    /* There seems to be a bug with sRGB default framebuffer, as
     * the buffer is always linear, but if we enable this the
     * drivers are doing the linear->sRGB conversion anyway!
     * So don't enable it for now, we will have to compensate
     * in the shaders */
    //GL( glEnable(GL_FRAMEBUFFER_SRGB) );
	GL( glCullFace(GL_BACK) );
    GL( glDisable(GL_DITHER) );
    GL( glDisable(GL_LINE_SMOOTH) );
    GL( glDisable(GL_POLYGON_SMOOTH) );
    GL( glHint(GL_POLYGON_SMOOTH_HINT, GL_DONT_CARE) );
#define GL_MULTISAMPLE_ARB 0x809D
    GL( glDisable(GL_MULTISAMPLE_ARB)  );
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDepthRangef(0.1f, 1000.0f);
}

const char *OpenGLRenderer::getName()
{
    return (const char*)glGetString(GL_RENDERER);
}

const char *OpenGLRenderer::getVersion()
{
    return (const char*)glGetString(GL_VERSION);
}

const char *OpenGLRenderer::getVendor()
{
    return (const char*)glGetString(GL_VENDOR);
}

const char *OpenGLRenderer::getShaderVersion()
{
    return (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
}

Shader * OpenGLRenderer::newShader(void)
{
	return new OpenGLShader();
}

RendererModel3D *OpenGLRenderer::prepareModel3D(const Model3D &model)
{
    OpenGLModel3D *glObject = new OpenGLModel3D();
    if (glObject != NULL) {
        glObject->init(model);
    }
    return glObject;
}

bool OpenGLRenderer::renderModel3D(RendererModel3D &model3D, Camera &camera,
                                   LightingShader &shader,
								   DirectLight *sun,
								   std::vector<PointLight*> &lights, float ambientK,
                                   RenderTarget &renderTarget, bool disableDepth)
{
	uint32_t numLights = 0;
	glm::mat4 biasMatrix(
			0.5, 0.0, 0.0, 0.0,
			0.0, 0.5, 0.0, 0.0,
			0.0, 0.0, 0.5, 0.0,
			0.5, 0.5, 0.5, 1.0
			);

	/* Calculate MVP matrix */
	glm::mat4 MVP = camera.getPerspectiveMatrix() * camera.getViewMatrix() * model3D.getModelMatrix();

    /* Calculate normal matrix */
    glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model3D.getModelMatrix())));

    /* Cast the model into an internal type */
    OpenGLModel3D &glObject = dynamic_cast<OpenGLModel3D&>(model3D);

    if (disableDepth) {
        glDisable(GL_DEPTH_TEST);
    } else {
        glEnable(GL_DEPTH_TEST);
    }
    /* Bind the render target */
    renderTarget.bind();
    {
        GL( glEnable(GL_MULTISAMPLE) );

        /* Bind program to upload the uniform */
        shader.attach();

        /* Send our transformation to the currently bound shader, in the "MVP" uniform */
        shader.setUniformMat4("u_MVPMatrix", MVP);
        shader.setUniformMat4("u_viewMatrix", camera.getViewMatrix());
        shader.setUniformMat4("u_modelMatrix", model3D.getModelMatrix());
        shader.setUniformMat3("u_normalMatrix", normalMatrix);
        shader.setUniformTexture2D("u_diffuseMap", 0);
        shader.setUniformFloat("u_ambientK", ambientK);

		/* Set the sun light */
		if (sun != NULL) {
			glm::mat4 shadowMVP = sun->getOrthogonalMatrix() *
				                  sun->getViewMatrix() *
								  model3D.getModelMatrix();
			shadowMVP = biasMatrix * shadowMVP;

			shader.setDirectLight(*sun);
			shader.setUniformUint("u_numDirectLights", 1);

			/* TODO: This has to be set in a matrix array */
			shader.setUniformMat4("u_shadowMVPMatrix", shadowMVP);
			shader.setUniformTexture2D("u_shadowMap", 1);

			GL( glActiveTexture(GL_TEXTURE1) );
			sun->getShadowMap()->bindDepth();
		} else {
			shader.setUniformUint("u_numDirectLights", 0);
		}

#if 0
		for (numLights=0; numLights<lights.size(); ++numLights) {
			shader.setPointLight(numLights, *lights[numLights]);

			/* Calculate adjusted shadow map matrix */
			if (numLights == 0) {
				glm::mat4 shadowMVP = lights[numLights]->getOrthogonalMatrix() *
									  lights[numLights]->getViewMatrix() *
									  model3D.getModelMatrix();
				shadowMVP = biasMatrix * shadowMVP;

				/* TODO: This has to be set in a matrix array */
				shader.setUniformMat4("u_shadowMVPMatrix", shadowMVP);
				shader.setUniformTexture2D("u_shadowMap", numLights+1);

				GL( glActiveTexture(GL_TEXTURE1) );
				lights[numLights]->getShadowMap()->bindDepth();
			}
		}
        shader.setUniformUint("u_numPointLights", numLights);
#else
        shader.setUniformUint("u_numPointLights", 0);
#endif

        /* Draw the model */
        GL( glBindVertexArray(glObject.getVertexArrayID()) );
        {
			GL( glActiveTexture(GL_TEXTURE0) );

            std::vector<Material> materials   = glObject.getMaterials();
            std::vector<uint32_t> texturesIDs = glObject.getTextures();
            std::vector<uint32_t> offset      = glObject.getIndicesOffsets();
            std::vector<uint32_t> count       = glObject.getIndicesCount();

            for (size_t  i=0; i<materials.size(); ++i) {
                GL( glBindTexture(GL_TEXTURE_2D, texturesIDs[i]) );
                shader.setMaterial(materials[i]);

                GL( glDrawElements(GL_TRIANGLES, count[i], GL_UNSIGNED_INT, (void*)(offset[i] * sizeof(GLuint))) );
            }
        }
        GL( glBindVertexArray(0) );

        /* Unbind */
        shader.detach();
    }
    renderTarget.unbind();

    return true;
}

bool OpenGLRenderer::renderToShadowMap(RendererModel3D &model3D, Light &light, NormalShadowMapShader &shader)
{
	/* Calculate MVP matrix */
	glm::mat4 MVP = light.getOrthogonalMatrix() * light.getViewMatrix() * model3D.getModelMatrix();

    /* Calculate normal matrix */
    glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model3D.getModelMatrix())));

    /* Cast the model into an internal type */
    OpenGLModel3D &glObject = dynamic_cast<OpenGLModel3D&>(model3D);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

    /* Bind the render target */
    light.getShadowMap()->bind();
    {
        /* Bind program to upload the uniform */
        shader.attach();

        /* Send our transformation to the currently bound shader, in the "MVP" uniform */
        shader.setUniformMat4("u_MVPMatrix", MVP);

        /* Draw the model */
        GL( glBindVertexArray(glObject.getVertexArrayID()) );
        {
            std::vector<uint32_t> offset      = glObject.getIndicesOffsets();
            std::vector<uint32_t> count       = glObject.getIndicesCount();

            for (size_t i=0; i<count.size(); ++i) {
                GL( glDrawElements(GL_TRIANGLES, count[i], GL_UNSIGNED_INT, (void*)(offset[i] * sizeof(GLuint))) );
            }
        }
        GL( glBindVertexArray(0) );

        /* Unbind */
        shader.detach();
    }
    light.getShadowMap()->unbind();

    return true;
}

bool OpenGLRenderer::resize(uint16_t width, uint16_t height)
{
	_width  = width;
	_height = height;
    return true;
}

void OpenGLRenderer::flush()
{
    glFinish();
}
