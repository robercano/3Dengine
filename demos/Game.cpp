/**
 * @class	FXAA/FXAA2 (custom implementation) comparison
 * @brief	Demo to compare Timothy Lottes FXAA implementation against mine
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include <string>
#if defined(__LINUX__)
#include <sys/time.h>
#include <unistd.h>

uint32_t GetTickCount(void)
{
	struct timeval t;
	gettimeofday(&t, NULL);
	return t.tv_sec * 1000 + t.tv_usec / 1000;
}

#elif defined (_WIN32) || defined(_WIN64)
#include <Windows.h>
#endif
#include "OpenGL.h" // For GLFW_KEY_ESCAPE
#include "OBJFormat.hpp"
#include "Shader.hpp"
#include "Light.hpp"
#include "WindowManager.hpp"
#include "InputManager.hpp"
#include "Renderer.hpp"
#include "FlyCamera.hpp"
#include "RenderTarget.hpp"
#include "TextConsole.hpp"
#include "NOAARenderTarget.hpp"
#include "MSAARenderTarget.hpp"
#include "SSAARenderTarget.hpp"
#include "FXAARenderTarget.hpp"
#include "FXAA2RenderTarget.hpp"

#define TARGET_FPS 60

class Game {
#if 0
/* Create a render target to allow post-processing */
renderTargetNOAA = NOAARenderTarget::New();
if (renderTargetNOAA == NULL) {
    fprintf(stderr, "ERROR allocating render target\n");
    return 1;
}
renderTargetFXAA = FXAARenderTarget::New();
if (renderTargetFXAA == NULL) {
    fprintf(stderr, "ERROR allocating render target\n");
    return 1;
}
renderTargetFXAA2 = FXAA2RenderTarget::New();
if (renderTargetFXAA2 == NULL) {
    fprintf(stderr, "ERROR allocating render target\n");
    return 1;
}

renderTargetNOAA->init(width/2, height);
renderTargetFXAA->init(width/2, height);
renderTargetFXAA2->init(width/2, height);

selectedTargetLeft = renderTargetNOAA;
selectedTargetRight = renderTargetNOAA;

/* Register the key and mouse listener */
std::vector<uint32_t> keys; // The keys should be read from a config file

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

windowManager->getKeyManager()->registerListener(inputManager, keys);
windowManager->getMouseManager()->registerListener(inputManager);

/* Create a Blinn-phong shader for the geometry */
shader = Shader::New();

std::string error;
if (shader->loadVertexShader("data/shaders/lighting/blinnphong_reflection.vert", error) == false) {
    printf("ERROR compiling vertex shader: %s\n", error.c_str());
    return 1;
}
if (shader->loadFragmentShader("data/shaders/lighting/blinnphong_reflection.frag", error) == false) {
    printf("ERROR compiling fragment shader: %s\n", error.c_str());
    return 1;
}
if (shader->linkProgram(error) == false) {
    printf("ERROR linking shader: %s\n", error.c_str());
    return 1;
}

    /* Load the geometry */
    std::string meshPath = "data/objects/deadpool";

    if (obj3D.load(meshPath) == false) {
        printf("ERROR loading OBJ file\n");
        return 1;
    }

	/* Wrap the geometry for the renderer, this typically generates any
     * renderer API specific structures and uploads data to the graphics card */
    RendererObject3D *renderObject = renderer->prepareObject3D(obj3D);

    objects.push_back(renderObject);
    shaders.push_back(shader);
#endif

    Game(std::string &name) : _name(name) {}

    void setWindowSize(uint32_t width, uint32_t height, bool fullscreen = false)
    {
        _width = width;
        _height = height;
        _fullscreen = fullscreen;
    }

    void setFPS(uint32_t FPS, bool unbound = false)
    {
        _dueTime = 1000.0/FPS;
        _unboundFPS = true;
    }

    bool init()
    {
        _windowManager = WindowManager::GetInstance();
        if (_windowManager == NULL) {
            fprintf(stderr, "ERROR creating new window manager\n");
            return false;
        }

        /* Use our system renderer. This only supports OpenGL for now */
        _renderer = Renderer::GetInstance();
        if (_renderer == NULL) {
            fprintf(stderr, "ERROR allocating renderer\n");
            return false;
        }

        /* Init the window manager and the render*/
        _windowManager->init();

        /* Set the window size */
        _windowManager->createWindow(_name, _width, _height, _fullscreen);
        _windowManager->getWindowSize(&_width, &_height);

        _renderer->init();	// only after creating the window

        _windowManager->setRenderer(_renderer);

        /* Setup the text console */
        glm::vec4 color(1.0, 0.5, 0.2, 1.0);
        std::string fontPath = "data/fonts/Arial.ttf";

        if (_console.init(fontPath, 14, _width, height) == false) {
            printf("ERROR creating text console\n");
            return 1;
        }

        _console.setForegroundColor(1.0, 0.5, 0.2, 1.0);
        _console.setBackgroundColor(0.0, 0.0, 0.0, 0.0);

        /* Create the game camera */
        _camera = new FlyCamera();
        _camera->setProjection(45, width/(2.0*(float)height), 0.1, 1000.0);
        glm::vec4 pos(220, 135, -1, 1);
        _camera->setPosition(pos);
        _camera->rotateYaw(-90);

        return true;
    }

    bool loop()
    {
        /* Main loop */
        gettimeofday(&inputNow, NULL);
        renderBegin = renderEnd = inputNow;

        while (true)
        {
            /* Read input */
            _windowManager->poll();

if (_inputManager._keys[GLFW_KEY_ESCAPE]) {
    break;
}

            /* Get elapsed time */
            inputPrevious = inputNow;
            gettimeofday(&inputNow, NULL);
            double inputElapsedMs = (inputNow.tv_sec - inputPrevious.tv_sec)*1000.0 +
                (inputNow.tv_usec - inputPrevious.tv_usec)/1000.0;

            /* Dispatch input to geometry */
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
            if (inputManager._keys['R']) {
                resetStats = true;
            }
            if (inputManager._keys['F']) {
                selectedTargetLeft = renderTargetFXAA;
                selectedTargetRight = renderTargetFXAA2;
                targets = "FXAA Left / FXAA2 Right";
            }
            if (inputManager._keys['N']) {
                selectedTargetLeft = renderTargetNOAA;
                selectedTargetRight = renderTargetNOAA;
                targets = "NOAA Left / NOAA Right";
            }
            if (resetStats) {
                minRenderFrameMs = 1000000.0f;
                maxRenderFrameMs = 0.0f;
                for (i=0; i<TARGET_FPS; ++i) {
                    avgRenderMs[i] = 1000.0;
                }
                resetStats = false;
            }

            if (prevX == 0xFFFFFF) {
                prevX = inputManager._xMouse;
            }
            if (prevY == 0xFFFFFF) {
                prevY = inputManager._yMouse;
            }

            int32_t diffMouseX = inputManager._xMouse - prevX;
            int32_t diffMouseY = InvertMouse*(inputManager._yMouse - prevY);

            if (diffMouseX) {
                _camera->rotateYaw(MouseSensibility*M_PI*diffMouseX/width);
            }
            if (diffMouseY) {
                _camera->rotatePitch(MouseSensibility*M_PI*diffMouseY/height);
            }

            prevX = inputManager._xMouse;
            prevY = inputManager._yMouse;

            /* If frame is due, render it */
            gettimeofday(&renderBegin, NULL);
            double renderElapsedMs = (renderBegin.tv_sec - renderEnd.tv_sec)*1000.0 +
                (renderBegin.tv_usec - renderEnd.tv_usec)/1000.0;

            /* We take into account the last render time to start the rendering process
             * in advance, so we are in time for the blit. This is currently too tight,
             * we should account for variations in the render time due to changes of the
             * game engine assets (camera, rendering distance, geometry) and also to
             * account for the jitter in the GPU/CPU when rendering the same scene */
            if (_unboundFPS == true || (renderElapsedMs + totalAvgTime*jitterAdj >= dueTime)) {

                /* Render all objects */
                for (i=0; i<objects.size(); ++i) {
                    renderer->renderObject3D(*objects[i], *shaders[i], lights, 0.0,
                            _camera->getProjection(), _camera->getView(),
                            *selectedTargetLeft);
                    renderer->renderObject3D(*objects[i], *shaders[i], lights, 0.0,
                            _camera->getProjection(), _camera->getView(),
                            *selectedTargetRight);
                }

                console.clear();
                console.gprintf("%s\n", targets.c_str());
                console.gprintf("FPS: %d\n", (int)FPS);
                console.gprintf("Upper FPS: %d\n", (int)(1000.0/totalAvgTime));
                console.gprintf("Avg. Render: %.2fms (%.2fms)\n", totalAvgTime, dueTime);

                selectedTargetLeft->blit(0, 0, width/2, height);
                selectedTargetRight->blit(width/2, 0, width/2, height);
                console.blit();

                /* Flush all operations so we can have a good measure
                 * of the time it takes to render the scene. Otherwise this
                 * will only happen when swapBuffers() is called. In a proper
                 * engine you want to accumulate operations and have a measurement
                 * of how long the card will take to render the scene, so you can
                 * maximimize the number of operations you can issue before running
                 * out of time:
                 *
                 *  |------------------------------ framerate time ----------------------------|
                 *  |__________________________________________________________________________|
                 *  |- process input -|- prepare scene -|- send rendering commands -|- render -|
                 *  |--------------------------------------------------------------------------|
                 *
                 *  Obviously not at scale. Rendering takes much longer than the rest.
                 */
                renderer->flush();
                windowManager->swapBuffers();

                /* Calculate how much did we take to render this frame */
                gettimeofday(&renderEnd, NULL);

                renderFrameMs = (renderEnd.tv_sec - renderBegin.tv_sec)*1000.0 +
                    (renderEnd.tv_usec - renderBegin.tv_usec)/1000.0;

                /* Check if we are really late, that means we are dropping
                 * frames. In this case re-adjust the render adjustment and
                 * the elapsed time. Have to check how costly that fmod is... */
                if (renderFrameMs > dueTime*jitterAdj) {
                    fprintf(stderr, "Droping frames...(%.2f, %.2f)\n", renderFrameMs, dueTime);
                }

                /* Calculate FPS now */
                if (renderFrameMs > maxRenderFrameMs) {
                    maxRenderFrameMs = renderFrameMs;
                }
                if (renderFrameMs < minRenderFrameMs) {
                    minRenderFrameMs = renderFrameMs;
                }

                avgRenderMs[avgRenderMsIdx] = renderFrameMs;
                avgRenderMsIdx = (avgRenderMsIdx + 1) % TARGET_FPS;

                /* Calculate the average FPS */
                for (i=0; i<TARGET_FPS; ++i) {
                    totalAvgTime += avgRenderMs[i];
                }
                totalAvgTime /= TARGET_FPS;

                /* Render elapsed time should be also averaged to get a good
                 * estimation, but for now this will do */
                FPS = (1000.0/(totalAvgTime + renderElapsedMs));
            }
        }

        fprintf(stderr, "Average FPS: %d\n", (int)(1000.0/totalAvgTime));
        fprintf(stderr, "Minimum FPS: %d\n", (int)(1000.0/maxRenderFrameMs));
        fprintf(stderr, "Maximum FPS: %d\n", (int)(1000.0/minRenderFrameMs));
        return 0;
    }

private:
    std::string gameName;
    WindowManager *windowManager = NULL;
    Renderer *renderer = NULL;
    InputManager inputManager;
    Camera *_camera;
    TextConsole console;
    OBJFormat obj3D;

    const float MouseSensibility = 10.0;
    const float InvertMouse = 1.0;
    int32_t prevX = 0xFFFFFF, prevY = 0xFFFFFF;
    struct timeval renderBegin, renderEnd;
    struct timeval inputNow, inputPrevious;
    double renderFrameMs;
    double jitterAdj = 1.02;
    bool resetStats = false;

    float avgRenderMs[TARGET_FPS] = { 1000.0 };
    uint32_t avgRenderMsIdx = 0;
    float minRenderFrameMs = 10000000.0f;
    float maxRenderFrameMs = 0.0f;
    float totalAvgTime = 0;
    float renderAdjustment = 0;
    float dueTime;
    float FPS;
    bool _unboundFPS;
    uint32_t width;
    uint32_t height;
    uint32_t i;
}
