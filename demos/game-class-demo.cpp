#include <stdio.h>
#include "Game.hpp"
#include "OBJFormat.hpp"
#include "OpenGL.h"
#include "Shader.hpp"
#include "NOAARenderTarget.hpp"
#include "MSAARenderTarget.hpp"
#include "SSAARenderTarget.hpp"
#include "FXAARenderTarget.hpp"
#include "FXAA2RenderTarget.hpp"

class AntiaAliasingDemo : public GameHandler
{
    public:
        AntiaAliasingDemo() {
            _MouseSensibility = 10.0;
        }

        bool handleInit(WindowManager &windowManager, Renderer &renderer, std::vector<uint32_t> &keys)
        {
            Light light1(glm::vec3(5.0, 5.0, 5.0),
                    glm::vec3(5.0, 5.0, 5.0),
                    glm::vec3(5.0, 5.0, 5.0),
                    glm::vec3(50.0, 100.0, 50.0));
            Light light2(glm::vec3(5.0, 3.0, 3.0),
                    glm::vec3(5.0, 3.0, 3.0),
                    glm::vec3(5.0, 3.0, 3.0),
                    glm::vec3(-50.0, 100.0, 50.0));
            Light light3(glm::vec3(1.0, 1.0, 3.0),
                    glm::vec3(1.0, 1.0, 3.0),
                    glm::vec3(1.0, 1.0, 3.0),
                    glm::vec3(50.0, 200.0, 100.0));

            _lights.push_back(&light1);
            _lights.push_back(&light2);
            _lights.push_back(&light3);

            windowManager.getWindowSize(&_width, &_height);

            /* Create a render target to allow post-processing */
            _renderTargetNOAA = NOAARenderTarget::New();
            if (_renderTargetNOAA == NULL) {
                fprintf(stderr, "ERROR allocating render target\n");
                return false;
            }

            _renderTargetNOAA->init(_width, _height);

            /* Register the key and mouse listener */
            keys.push_back('W');
            keys.push_back('S');
            keys.push_back('A');
            keys.push_back('D');
            keys.push_back('R');
            keys.push_back('N');
            keys.push_back('F');
            keys.push_back('Q');
            keys.push_back('E');
            keys.push_back(GLFW_KEY_ESCAPE);

            /* Create a Blinn-phong shader for the geometry */
            _shader = Shader::New();

            std::string error;
            if (_shader->loadVertexShader("data/shaders/lighting/blinnphong_reflection.vert", error) == false) {
                printf("ERROR compiling vertex shader: %s\n", error.c_str());
                return false;
            }
            if (_shader->loadFragmentShader("data/shaders/lighting/blinnphong_reflection.frag", error) == false) {
                printf("ERROR 00compiling fragment shader: %s\n", error.c_str());
                return false;
            }
            if (_shader->linkProgram(error) == false) {
                printf("ERROR linking shader: %s\n", error.c_str());
                return false;
            }

            /* Load the geometry */
            std::string meshPath = "data/objects/deadpool";

            OBJFormat obj3D;
            if (obj3D.load(meshPath) == false) {
                printf("ERROR loading OBJ file\n");
                return false;
            }

            /* Wrap the geometry for the renderer, this typically generates any
             * renderer API specific structures and uploads data to the graphics card */
            _model3D = renderer.prepareObject3D(obj3D);

            /* Create the game camera */
            _camera = new FlyCamera();
            _camera->setProjection(45, _width/(float)_height, 0.1, 1000.0);
            glm::vec4 pos(220, 135, -1, 1);
            _camera->setPosition(pos);
            _camera->rotateYaw(-90);

            return true;
        }

        bool handleKeyboard(InputManager &inputManager, double inputElapsedMs)
        {
            if (inputManager._keys[GLFW_KEY_ESCAPE]) {
                return false;
            }
            if (inputManager._keys['W']) {
                _camera->forward(0.1*inputElapsedMs);
            }
            if (inputManager._keys['S']) {
                _camera->forward(-0.1*inputElapsedMs);
            }
            if (inputManager._keys['A']) {
                _camera->right(-0.1*inputElapsedMs);
            }
            if (inputManager._keys['D']) {
                _camera->right(0.1*inputElapsedMs);
            }

            return true;
        }

        bool handleMouse(int32_t diffMouseX, int32_t diffMouseY)
        {
            if (diffMouseX) {
                _camera->rotateYaw(_MouseSensibility*M_PI*diffMouseX/_width);
            }
            if (diffMouseY) {
                _camera->rotatePitch(_MouseSensibility*M_PI*diffMouseY/_height);
            }

            return true;
        }

        bool handleRender(Renderer &renderer, TextConsole &console)
        {
            /* Render all objects */
            renderer.renderObject3D(*_model3D, *_shader, _lights, 0.0,
                    _camera->getProjection(), _camera->getView(),
                    *_renderTargetNOAA);

            _renderTargetNOAA->blit(0, 0, _width, _height);
            return true;
        }

    private:
        Camera    *_camera;
        RendererObject3D *_model3D;
        Shader    *_shader;
        NOAARenderTarget *_renderTargetNOAA;
        std::vector<Light*> _lights;

        float _MouseSensibility;
        uint32_t _width;
        uint32_t _height;
};

int main()
{
    Game *game;
    AntiaAliasingDemo antiAliasingDemo;

    game = new Game("Anti Aliasing Comparison demo");
    if (game == NULL) {
        fprintf(stderr, "ERROR creating new game\n");
        return 1;
    }

    game->setHandler(&antiAliasingDemo);
    game->setWindowSize(800, 600, false);
    game->setFPS(60);

    if (game->init() == false) {
        fprintf(stderr, "ERROR initializing game\n");
        return 1;
    }

    game->loop();
    return 0;
}
