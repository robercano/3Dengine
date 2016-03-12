/**
 * @class	Anti-aliasing comparison
 * @brief	Demo to compare different aliasing methods like MSAA, SSAA and FXAA
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include <string>
#include <sys/time.h>
#include <unistd.h>
#include "OpenGL.h" // For GLFW_KEY_ESCAPE
#include "OBJFormat.hpp"
#include "Shader.hpp"
#include "ShaderMaterial.hpp"
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

int main(int argc, char**argv)
{
    WindowManager *windowManager = NULL;
    Renderer *renderer = NULL;
    RenderTarget *selectedTargetLeft = NULL;
    RenderTarget *selectedTargetRight = NULL;
    NOAARenderTarget *renderTargetNOAA = NULL;
    FXAARenderTarget *renderTargetFXAA = NULL;
    FXAA2RenderTarget *renderTargetFXAA2 = NULL;
    InputManager inputManager;
    Camera *_camera;
    std::vector<RendererObject3D*> objects;
    std::vector<Shader*> shaders;
    std::string gameName;
    std::string targets = "NOAA Left / NOAA Right";
    TextConsole console;
    OBJFormat obj3D;
    Shader *shader = NULL;

	const float MouseSensibility = 10.0;
	const float InvertMouse = 1.0;
    float FPS = 0.0;
	int32_t prevX = 0xFFFFFF, prevY = 0xFFFFFF;
	struct timeval renderNow, renderPrevious;
    struct timeval inputNow, inputPrevious;
    bool resetStats = false;

    float avgFPS[TARGET_FPS] = {0};
    uint32_t avgFPSIdx = 0;
    float minFPS = 10000000.0f;
    float maxFPS = 0.0f;
    float totalAvgFPS = 0;
	uint32_t renders=0;
    float renderAdjustment = 0;
    float dueTime;
    bool _unboundFPS;
    uint32_t width;
    uint32_t height;
    uint32_t i;

	/* TODO: Get the settings from a config file */
    width = 1920;
    height = 1080;

    dueTime = 1000.0/TARGET_FPS;
    _unboundFPS = true;

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
    gameName = "FXAA comparison";
	windowManager->createWindow(gameName, width, height, true);
    windowManager->getWindowSize(&width, &height);

	renderer->init();	// only after creating the window
    renderTargetNOAA->init(width/2, height);
    renderTargetFXAA->init(width/2, height);
    renderTargetFXAA2->init(width/2, height);

    selectedTargetLeft = renderTargetNOAA;
    selectedTargetRight = renderTargetNOAA;

	windowManager->setRenderer(renderer);

    /* Setup the text console */
    glm::vec4 color(1.0, 0.5, 0.2, 1.0);
    std::string fontPath = "data/fonts/Arial.ttf";

    if (console.init(fontPath, 14, width, height) == false) {
        printf("ERROR creating text console\n");
        return 1;
    }

    console.setForegroundColor(1.0, 0.5, 0.2, 1.0);
    console.setBackgroundColor(0.0, 0.0, 0.0, 0.0);

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

	/* Create the game camera */
	_camera = new FlyCamera();
	_camera->setProjection(45, width/(2.0*(float)height), 0.1, 1000.0);
    glm::vec4 pos(220, 135, -1, 1);
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
	gettimeofday(&inputNow, NULL);
    renderNow = renderPrevious = inputNow;

	while (true)
	{
		/* Read input */
		windowManager->poll();
		if (inputManager._keys[GLFW_KEY_ESCAPE]) {
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
            minFPS = 1000000.0f;
            maxFPS = 0.0f;
            for (i=0; i<TARGET_FPS; ++i) {
                avgFPS[i] = FPS;
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

        /* Render all objects */
        for (i=0; i<objects.size(); ++i) {
            renderer->renderObject3D(*objects[i], *shaders[i],
                    _camera->getProjection(), _camera->getView(),
                    *selectedTargetLeft);
            renderer->renderObject3D(*objects[i], *shaders[i],
                    _camera->getProjection(), _camera->getView(),
                    *selectedTargetRight);
        }

        console.clear();
        console.gprintf("%s\n", targets.c_str());
        console.gprintf("Avg. FPS: %d\n", (int)totalAvgFPS);

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
        renders++;

		/* If frame is due, blit it */
		gettimeofday(&renderNow, NULL);
		double renderElapsedMs = (renderNow.tv_sec - renderPrevious.tv_sec)*1000.0 +
                                 (renderNow.tv_usec - renderPrevious.tv_usec)/1000.0;

        /* Render time! */
		if ((renderAdjustment + renderElapsedMs) >= dueTime) {
			windowManager->swapBuffers();
            renderPrevious = renderNow;

            /* Check if we are really late, that means we are dropping
             * frames. In this case re-adjust the render adjustment and
             * the elapsed time. Have to check how costly that fmod is... */
            if ((renderAdjustment + renderElapsedMs) > 2*dueTime) {
                renderElapsedMs = fmod(renderElapsedMs, dueTime) + dueTime;
                renderAdjustment = fmod(renderAdjustment, dueTime);
                fprintf(stderr, "Droping frames...\n");
            }
            renderAdjustment = renderAdjustment + renderElapsedMs - dueTime;

            /* Calculate FPS now */
            FPS = 1000.0*renders/renderElapsedMs;

            if (FPS > maxFPS) {
                maxFPS = FPS;
            }
            if (FPS < minFPS) {
                minFPS = FPS;
            }

            avgFPS[avgFPSIdx] = FPS;
            avgFPSIdx = (avgFPSIdx + 1)%TARGET_FPS;

            /* Calculate the average FPS */
            for (i=0; i<TARGET_FPS; ++i) {
                totalAvgFPS += avgFPS[i];
            }
            totalAvgFPS /= TARGET_FPS;

            renders = 0;
        }
	}

    fprintf(stderr, "Average FPS: %d\n", (int)totalAvgFPS);
    fprintf(stderr, "Minimum FPS: %d\n", (int)minFPS);
    fprintf(stderr, "Maximum FPS: %d\n", (int)maxFPS);
	return 0;
}
