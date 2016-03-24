/**
 * @class	FXAA/FXAA2 (custom implementation) comparison
 * @brief	Demo to compare Timothy Lottes FXAA implementation against mine
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include "Game.hpp"

void Game::setWindowSize(uint32_t width, uint32_t height, bool fullscreen)
{
    _width = width;
    _height = height;
    _fullscreen = fullscreen;
}

void Game::setFPS(uint32_t FPS, bool unbound)
{
    _targetFPS = FPS;
    _unboundFPS = true;
}

bool Game::init()
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

    /* Time manager */
    _timer = TimeManager::GetInstance();
    if (_timer == NULL) {
        fprintf(stderr, "ERROR allocating time manager\n");
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

    if (_console.init(fontPath, 14, _width, _height) == false) {
        printf("ERROR creating text console\n");
        return 1;
    }

    _console.setForegroundColor(1.0, 0.5, 0.2, 1.0);
    _console.setBackgroundColor(0.0, 0.0, 0.0, 0.0);

    std::vector<uint32_t> keys; // The keys should be read from a config file
    _gameHandler->handleInit(*_windowManager, *_renderer, keys);

    _windowManager->getKeyManager()->registerListener(_inputManager, keys);
    _windowManager->getMouseManager()->registerListener(_inputManager);

    return true;
}

bool Game::loop()
{
    const float InvertMouse = 1.0;
    int32_t prevX = 0xFFFFFF, prevY = 0xFFFFFF;
    double renderBegin, renderEnd;
    double inputNow, inputPrevious;
    double renderFrameMs;
    double jitterAdj = 1.02;
    bool resetStats = false;
    float *avgRenderMs = NULL;
    uint32_t avgRenderMsIdx = 0;
    float minRenderFrameMs = 10000000.0f;
    float maxRenderFrameMs = 0.0f;
    float totalAvgTime = 0;
    float renderAdjustment = 0;
    float dueTime = 1000.0/_targetFPS;
    float FPS;
    int i;

    /* Main loop */
    avgRenderMs = new float[_targetFPS]();

    inputNow = _timer->getElapsedMs();
    renderBegin = renderEnd = inputNow;

    while (true)
    {
        /* Read input */
        _windowManager->poll();

        /* Get elapsed time */
        inputPrevious = inputNow;
        inputNow = _timer->getElapsedMs();
        double inputElapsedMs = inputNow - inputPrevious;

        if (_gameHandler->handleKeyboard(_inputManager, inputElapsedMs) != true) {
            break;
        }

        if (resetStats) {
            minRenderFrameMs = 1000000.0f;
            maxRenderFrameMs = 0.0f;
            for (i=0; i<_targetFPS; ++i) {
                avgRenderMs[i] = 1000.0;
            }
            resetStats = false;
        }

        if (prevX == 0xFFFFFF) {
            prevX = _inputManager._xMouse;
        }
        if (prevY == 0xFFFFFF) {
            prevY = _inputManager._yMouse;
        }

        int32_t diffMouseX = _inputManager._xMouse - prevX;
        int32_t diffMouseY = InvertMouse*(_inputManager._yMouse - prevY);

        if (_gameHandler->handleMouse(diffMouseX, diffMouseY) != true) {
            fprintf(stderr, "ERROR calling mouse callback\n");
        }

        prevX = _inputManager._xMouse;
        prevY = _inputManager._yMouse;

        /* If frame is due, render it */
        renderBegin = _timer->getElapsedMs();
        double renderElapsedMs = renderBegin - renderEnd;

        /* We take into account the last render time to start the rendering process
         * in advance, so we are in time for the blit. This is currently too tight,
         * we should account for variations in the render time due to changes of the
         * game engine assets (camera, rendering distance, geometry) and also to
         * account for the jitter in the GPU/CPU when rendering the same scene */
        if (_unboundFPS == true || (renderElapsedMs + totalAvgTime*jitterAdj >= dueTime)) {

            _console.clear();
            if (_gameHandler->handleRender(*_renderer, _console) != true) {
                fprintf(stderr, "ERROR handling render callback");
            }
            _console.gprintf("FPS: %d\n", (int)FPS);
            _console.gprintf("Upper FPS: %d\n", (int)(1000.0/totalAvgTime));
            _console.gprintf("Avg. Render: %.2fms (%.2fms)\n", totalAvgTime, dueTime);
            _console.blit();

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
            _renderer->flush();
            _windowManager->swapBuffers();

            /* Calculate how much did we take to render this frame */
            renderEnd = _timer->getElapsedMs();
            renderFrameMs = renderEnd - renderBegin;

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
            avgRenderMsIdx = (avgRenderMsIdx + 1) % _targetFPS;

            /* Calculate the average FPS */
            for (i=0; i<_targetFPS; ++i) {
                totalAvgTime += avgRenderMs[i];
            }
            totalAvgTime /= _targetFPS;

            /* Render elapsed time should be also averaged to get a good
             * estimation, but for now this will do */
            FPS = (1000.0/(totalAvgTime + renderElapsedMs));
        }
    }

    fprintf(stderr, "Average FPS: %d\n", (int)(1000.0/totalAvgTime));
    fprintf(stderr, "Minimum FPS: %d\n", (int)(1000.0/maxRenderFrameMs));
    fprintf(stderr, "Maximum FPS: %d\n", (int)(1000.0/minRenderFrameMs));

    return true;
}
