/**
 * @class	Anti-aliasing comparison
 * @brief	Demo to compare different aliasing methods like MSAA, SSAA and FXAA
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

#define TARGET_FPS 59
#define PI 3.14159265358979323846f

int main(int argc, char**argv)
{
    WindowManager *windowManager = NULL;
    Renderer *renderer = NULL;
    NOAARenderTarget *renderTargetNOAA = NULL;
    MSAARenderTarget *renderTargetMSAA = NULL;
    SSAARenderTarget *renderTargetSSAA = NULL;
    FXAARenderTarget *renderTargetFXAA = NULL;
    FXAA2RenderTarget *renderTargetFXAA2 = NULL;
    RenderTarget *selectedRenderTarget = NULL;
    std::string renderTargetName;
    InputManager inputManager;
    Camera *_camera;
    std::vector<RendererObject3D*> objects;
    std::vector<Shader*> shaders;
    std::string gameName;
    TextConsole console;
    OBJFormat obj3D;
    Shader *shader = NULL;

	const float MouseSensibility = 10.0;
	const float InvertMouse = 1.0;
	int32_t prevX = 0xFFFFFF, prevY = 0xFFFFFF;
	uint32_t renderBegin, renderEnd;
    uint32_t inputNow, inputPrevious;
    uint32_t renderFrameMs;
    float jitterAdj = 1.02f;
    bool resetStats = false;

    float avgRenderMs[TARGET_FPS] = { 1000.0 };
    uint32_t avgRenderMsIdx = 0;
    uint32_t minRenderFrameMs = 10000000;
    uint32_t maxRenderFrameMs = 0;
    float totalAvgTime = 0;
    float renderAdjustment = 0;
    float dueTime;
    float FPS = 0.0f;
    bool _unboundFPS;
    uint32_t width;
    uint32_t height;
    uint32_t i;

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

	std::vector<Light*> lights;

	lights.push_back(&light1);
	lights.push_back(&light2);
	lights.push_back(&light3);

	/* TODO: Get the settings from a config file */
    width = 1920;
    height = 1080;

    dueTime = 1000.0f/TARGET_FPS;
    _unboundFPS = false;

	windowManager = WindowManager::GetInstance();
	if (windowManager == NULL) {
		fprintf(stderr, "ERROR creating new window manager\n");
		return 1;
	}

	/* Use our system renderer. This only supports OpenGL for now */
	renderer = Renderer::GetInstance();
	if (renderer == NULL) {
		fprintf(stderr, "ERROR allocating renderer\n");
		return 1;
	}

    /* Create a render target to allow post-processing */
    renderTargetNOAA = NOAARenderTarget::New();
	if (renderTargetNOAA == NULL) {
		fprintf(stderr, "ERROR allocating render target\n");
		return 1;
	}
    renderTargetMSAA = MSAARenderTarget::New();
	if (renderTargetMSAA == NULL) {
		fprintf(stderr, "ERROR allocating render target\n");
		return 1;
	}
    renderTargetSSAA = SSAARenderTarget::New();
	if (renderTargetSSAA == NULL) {
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

	/* Init the window manager and the render*/
	windowManager->init();

	/* Set the window size */
    gameName = "Antia-aliasing comparison";
	windowManager->createWindow(gameName, width, height, true);
    windowManager->getWindowSize(&width, &height);

	renderer->init();	// only after creating the window
    renderTargetNOAA->init(width, height);
    renderTargetMSAA->init(width, height, MSAARenderTarget::getMaxSamples() < 4 ?
                                             MSAARenderTarget::getMaxSamples() : 4);
    renderTargetSSAA->init(width, height, 4);
    renderTargetFXAA->init(width, height);
    renderTargetFXAA2->init(width, height);

    /* Choose the render target here */
    selectedRenderTarget = renderTargetNOAA;
    renderTargetName = "NOAA";

	windowManager->setRenderer(renderer);

    /* Setup the text console */
    glm::vec4 color(1.0, 0.5, 0.2, 1.0);
    std::string fontPath = "data/fonts/Arial.ttf";

    if (console.init(fontPath, 14, width, height) == false) {
        printf("ERROR creating text console\n");
        return 1;
    }

    console.setForegroundColor(1.0f, 0.5f, 0.2f, 1.0f);
    console.setBackgroundColor(0.0f, 0.0f, 0.0f, 0.0f);

	/* Register the key and mouse listener */
	std::vector<uint32_t> keys; // The keys should be read from a config file

	keys.push_back('W');
	keys.push_back('S');
	keys.push_back('A');
	keys.push_back('D');
	keys.push_back('1');
	keys.push_back('2');
	keys.push_back('3');
	keys.push_back('4');
	keys.push_back('5');
	keys.push_back('R');
	keys.push_back(GLFW_KEY_ESCAPE);

	windowManager->getKeyManager()->registerListener(inputManager, keys);
	windowManager->getMouseManager()->registerListener(inputManager);

	/* Create the game camera */
	_camera = new FlyCamera();
	_camera->setProjection(45, width/(float)height, 0.1f, 1000.0f);
    glm::vec4 pos( 220, 165, -1, 1);
    _camera->setPosition(pos);
    _camera->rotateYaw(-90);

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

    /* Main loop */
	inputNow = GetTickCount();
    renderBegin = renderEnd = inputNow;

	while (true)
	{
		/* Read input */
		windowManager->poll();
		if (inputManager._keys[GLFW_KEY_ESCAPE]) {
			break;
		}

		/* Get elapsed time */
		inputPrevious = inputNow;
		inputNow = GetTickCount();
		uint32_t inputElapsedMs = inputNow - inputPrevious;

		/* Dispatch input to geometry */
		if (inputManager._keys['W']) {
			_camera->forward(0.1f*inputElapsedMs);
		}
        if (inputManager._keys['S']) {
			_camera->forward(-0.1f*inputElapsedMs);
		}
        if (inputManager._keys['A']) {
			_camera->right(-0.1f*inputElapsedMs);
		}
        if (inputManager._keys['D']) {
			_camera->right(0.1f*inputElapsedMs);
		}
        if (inputManager._keys['1']) {
            selectedRenderTarget = renderTargetNOAA;
            renderTargetName = "NOAA";
            resetStats = true;
        } else if (inputManager._keys['2']) {
            selectedRenderTarget = renderTargetMSAA;
            renderTargetName = "MSAA";
            resetStats = true;
        } else if (inputManager._keys['3']) {
            selectedRenderTarget = renderTargetSSAA;
            renderTargetName = "SSAA";
            resetStats = true;
        } else if (inputManager._keys['4']) {
            selectedRenderTarget = renderTargetFXAA;
            renderTargetName = "FXAA";
            resetStats = true;
        } else if (inputManager._keys['5']) {
            selectedRenderTarget = renderTargetFXAA2;
            renderTargetName = "FXAA2";
            resetStats = true;
        }
        if (inputManager._keys['R']) {
            resetStats = true;
        }
        if (resetStats) {
            minRenderFrameMs = 1000000;
            maxRenderFrameMs = 0;
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
		int32_t diffMouseY = (int32_t)(InvertMouse*(inputManager._yMouse - prevY));

		if (diffMouseX) {
			_camera->rotateYaw(MouseSensibility*PI*diffMouseX/width);
		}
		if (diffMouseY) {
			_camera->rotatePitch(MouseSensibility*PI*diffMouseY/height);
		}

		prevX = inputManager._xMouse;
		prevY = inputManager._yMouse;

        /* If frame is due, render it */
        renderBegin = GetTickCount();
        uint32_t renderElapsedMs = renderBegin - renderEnd;

        /* We take into account the last render time to start the rendering process
         * in advance, so we are in time for the blit. This is currently too tight,
         * we should account for variations in the render time due to changes of the
         * game engine assets (camera, rendering distance, geometry) and also to
         * account for the jitter in the GPU/CPU when rendering the same scene */
        if (_unboundFPS == true || (renderElapsedMs + totalAvgTime*jitterAdj >= dueTime)) {
            /* Render all objects */
            for (i=0; i<objects.size(); ++i) {
                renderer->renderObject3D(*objects[i], *shaders[i],
                                         lights, 0.0,
                                         _camera->getProjection(),
                                         _camera->getView(),
                                         *selectedRenderTarget);
            }

            console.clear();
            console.gprintf("Anti-aliasing: %s\n", renderTargetName.c_str());
            console.gprintf("FPS: %d\n", (int)FPS);
            console.gprintf("Upper FPS: %d\n", (int)(1000.0/totalAvgTime));
            console.gprintf("Avg. Render: %.2fms (%.2fms)\n", totalAvgTime, dueTime);

            selectedRenderTarget->blit(0, 0, width, height);
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
            renderEnd = GetTickCount();

            renderFrameMs = renderEnd - renderBegin;

            /* Check if we are really late, that means we are dropping
             * frames. In this case re-adjust the render adjustment and
             * the elapsed time. Have to check how costly that fmod is... */
            if (renderFrameMs > dueTime*jitterAdj) {
                fprintf(stderr, "Droping frames...(%d, %.2f)\n", renderFrameMs, dueTime);
            }

            /* Calculate FPS now */
            if (renderFrameMs > maxRenderFrameMs) {
                maxRenderFrameMs = renderFrameMs;
            }
            if (renderFrameMs < minRenderFrameMs) {
                minRenderFrameMs = renderFrameMs;
            }

            avgRenderMs[avgRenderMsIdx] = (float)renderFrameMs;
            avgRenderMsIdx = (avgRenderMsIdx + 1) % TARGET_FPS;

            /* Calculate the average FPS */
            for (i=0; i<TARGET_FPS; ++i) {
                totalAvgTime += avgRenderMs[i];
            }
            totalAvgTime /= TARGET_FPS;

            /* Render elapsed time should be also averaged to get a good
             * estimation, but for now this will do */
            FPS = (1000.0f/(totalAvgTime + renderElapsedMs));
        }
	}

    fprintf(stderr, "Average FPS: %d\n", (int)(1000.0/totalAvgTime));
    fprintf(stderr, "Minimum FPS: %d\n", (int)(1000.0/maxRenderFrameMs));
    fprintf(stderr, "Maximum FPS: %d\n", (int)(1000.0/minRenderFrameMs));
	return 0;
}
