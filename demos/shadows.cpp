#include <stdio.h>
#include "Game.hpp"
#include "OBJFormat.hpp"
#include "OpenGL.h"
#include "ToonLightingShader.hpp"
#include "BlinnPhongShader.hpp"
#include "FXAA2RenderTarget.hpp"
#include "ToonRenderTarget.hpp"
#include "NOAARenderTarget.hpp"
#include "NormalShadowMapShader.hpp"
#include "Camera.hpp"
#include "FlyMotion.hpp"
#include "Plane.hpp"
#include "PointLight.hpp"
#include "DirectLight.hpp"

#define PI 3.14159265358979323846

class ShadowsDemo : public GameHandler
{
    public:
        ShadowsDemo() {
            _MouseSensibility = 10.0f;
			_KeyboardSensibility = 0.1f;
			_InvertMouse = 1.0;
			_prevX = 0xFFFFFF;
			_prevY = 0xFFFFFF;
			_angle = 0.0;
        }

        bool handleInit(Game *game)
        {
			PointLight *light1 = new PointLight(glm::vec3(3.5f, 3.5f, 4.5f),
									            glm::vec3(3.5f, 3.5f, 4.5f),
									            glm::vec3(3.5f, 3.5f, 4.5f),
									            glm::vec3(-100.0f, 100.0f, 100.0f),
												0.0000099999f,
												1000.0f);

			_sun = new DirectLight(glm::vec3(2.0f, 2.0f, 2.0f),
                                   glm::vec3(2.0f, 2.0f, 2.0f),
								   glm::vec3(2.0f, 2.0f, 2.0f),
								   glm::vec3(-100.0f, -100.0f, -100.0f));

            game->getWindowManager()->getWindowSize(&_width, &_height);

			/* Setup the normal render target for the shadow mapping */
			_renderTargetNormal = NOAARenderTarget::New();
			if (_renderTargetNormal == NULL) {
				fprintf(stderr, "ERROR allocating normal render target\n");
				return false;
			}

			_renderTargetNormal->init(_width, _height);
			_renderTargetNormal->setClearColor(0.0, 0.0, 0.0, 1.0);

            /* Register the key and mouse listener */
			std::vector<uint32_t> keys; // The keys should be read from a config file

            keys.push_back('W');
            keys.push_back('S');
            keys.push_back('A');
            keys.push_back('D');
            keys.push_back('R');
            keys.push_back('1');
            keys.push_back('2');
            keys.push_back('3');
            keys.push_back('4');
            keys.push_back('5');
            keys.push_back(GLFW_KEY_ESCAPE);

			game->getWindowManager()->getKeyManager()->registerListener(_inputManager, keys);
			game->getWindowManager()->getMouseManager()->registerListener(_inputManager);

            /* Create a Blinn-phong shader for the geometry */
            _shaderBlinnLight  = BlinnPhongShader::New();
			if (_shaderBlinnLight->init() == false) {
				printf("ERROR initializing toon lighting shader\n");
				return false;
			}
			_shaderShadow = NormalShadowMapShader::New();
			if (_shaderShadow->init() == false) {
				printf("ERROR initializing shadow map shader\n");
				return false;
			}

            /* Load the geometry */
            std::string meshPath = "data/objects/daxter";

            OBJFormat obj3D;
            if (obj3D.load(meshPath) == false) {
                printf("ERROR loading OBJ file\n");
                return false;
            }

            /* Wrap the geometry for the renderer, this typically generates any
             * renderer API specific structures and uploads data to the graphics card */
            _model3D = game->getRenderer()->prepareModel3D(obj3D);
            _model3D->setScaleFactor(glm::vec3(100.0f, 100.0f, 100.0f));

			/* Use a plane for the floor */
			procedural::Plane plane;

			_plane3D = game->getRenderer()->prepareModel3D(plane);
			_plane3D->setScaleFactor(glm::vec3(500.0f, 1.0f, 500.0f));

            /* Create the game camera */
            _camera.setProjection((float)_width, (float)_height, 0.1f, 1000.0f, 45.0f);
			_cameraMotion.setPosition( glm::vec3(150.0f, 100.0f, 150.0f) );
            _cameraMotion.rotateYaw(-45.0f);

			light1->setProjection((float)_width/4.0f, (float)_height/4.0f, 0.1f, 1000.0f);
			light1->getShadowMap()->init(_width, _height);

			/* Hack to properly calculate direct light frustum */
			_sun->setPosition(glm::vec3(100.0f, 100.0f, 100.0f));
			_sun->setProjection((float)_width/4.0f, (float)_height/4.0f, 0.1f, 1000.0f);
			_sun->getShadowMap()->init(_width, _height);

            //_lights.push_back(light1);

            return true;
        }

        bool handleTick(Game *game, double elapsedMs)
        {
			/* Keyboard */
            if (_inputManager._keys[GLFW_KEY_ESCAPE]) {
                return false;
            }
            if (_inputManager._keys['W']) {
                _cameraMotion.forward(_KeyboardSensibility*(float)elapsedMs);
            }
            if (_inputManager._keys['S']) {
                _cameraMotion.forward(-_KeyboardSensibility*(float)elapsedMs);
            }
            if (_inputManager._keys['A']) {
                _cameraMotion.right(-_KeyboardSensibility*(float)elapsedMs);
            }
            if (_inputManager._keys['D']) {
                _cameraMotion.right(_KeyboardSensibility*(float)elapsedMs);
            }
            if (_inputManager._keys['R']) {
                game->resetStats();
            }

			/* Mouse */
			if (_prevX == 0xFFFFFF) {
				_prevX = _inputManager._xMouse;
			}
			if (_prevY == 0xFFFFFF) {
				_prevY = _inputManager._yMouse;
			}

			int32_t diffMouseX = _inputManager._xMouse - _prevX;
			int32_t diffMouseY = (int32_t)(_InvertMouse*(_inputManager._yMouse - _prevY));

            if (diffMouseX) {
                _cameraMotion.rotateYaw((float)(_MouseSensibility*PI*diffMouseX/_width));
            }
            if (diffMouseY) {
                _cameraMotion.rotatePitch((float)(_MouseSensibility*PI*diffMouseY/_height));
            }

			_prevX = _inputManager._xMouse;
			_prevY = _inputManager._yMouse;

			/* Move the lights */
			_angle += (float)(2*PI*elapsedMs/5000.0);
			if (_angle > 2*PI) {
				_angle -= (float)(2*PI);
			}

			if (_lights.size() > 0) {
				_lights[0]->setPosition(glm::vec3(300.0*glm::sin(-_angle), 300.0, 300.0*glm::cos(-_angle)));
			}
			_sun->setPosition(glm::vec3(300.0*glm::sin(-_angle), 300.0, 300.0*glm::cos(-_angle)));

            return true;
        }

        bool handleRender(Game *game)
        {
			/* Apply the motion to the camera */
			_cameraMotion.applyTo(_camera);
            _renderTargetNormal->clear();

			_model3D->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
			_plane3D->setPosition(glm::vec3(0.0f, -70.0f, 0.0f));

			/* Render the shadow map for each light */
			for (std::vector<PointLight*>::iterator it = _lights.begin(); it != _lights.end(); ++it) {
				/* TODO: lookAt the center of the calculated bounding box, but for
				 * now this is enough */
				(*it)->lookAt(_model3D->getPosition());
				(*it)->getShadowMap()->clear();

				game->getRenderer()->renderToShadowMap(*_model3D, *(*it), *_shaderShadow);
			}

			/* Render the shadow map for the sun */
			_sun->lookAt(_model3D->getPosition());
			_sun->getShadowMap()->clear();
			game->getRenderer()->renderToShadowMap(*_model3D, *_sun, *_shaderShadow);

            /* Render all objects */
            game->getRenderer()->renderModel3D(*_model3D, _camera, *_shaderBlinnLight, _sun, _lights, 0.2f, *_renderTargetNormal);
            game->getRenderer()->renderModel3D(*_plane3D, _camera, *_shaderBlinnLight, _sun, _lights, 0.2f, *_renderTargetNormal);

			_renderTargetNormal->blit();
            return true;
        }

    private:
        Camera                 _camera;
		FlyMotion              _cameraMotion;
        RendererModel3D       *_model3D;
        RendererModel3D       *_plane3D;
        BlinnPhongShader      *_shaderBlinnLight;
		NormalShadowMapShader *_shaderShadow;
		NOAARenderTarget      *_renderTargetNormal;
        std::vector<PointLight*> _lights;
		DirectLight            *_sun;
		InputManager           _inputManager;
        std::string            _current;

        float _MouseSensibility;
        float _KeyboardSensibility;
		float _InvertMouse;
		int32_t _prevX;
		int32_t	_prevY;
        uint32_t _width;
        uint32_t _height;
		float _angle;
};

int main()
{
    Game *game;
    ShadowsDemo shadowsDemo;

    game = new Game("Anti Aliasing Comparison demo");
    if (game == NULL) {
        fprintf(stderr, "ERROR creating new game\n");
        return 1;
    }

    game->setHandler(&shadowsDemo);
#ifdef _WIN32
    game->setWindowSize(800, 600, false);
#else
    game->setWindowSize(2560, 1440, true);
#endif
    game->setFPS(60);

    if (game->init() == false) {
        fprintf(stderr, "ERROR initializing game\n");
        return 1;
   }

    game->loop();
    return 0;
}
