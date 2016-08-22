/**
 * @class	Viewport
 * @brief	Defines a viewport for rendering. The viewport will be used to determine the
 *          clip-coordinates to window projection. It allows to have several viewports
 *          on the main window
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <stdint.h>

class Viewport
{
  public:
    /**
     * Constructors
     */
    Viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) : _x(x), _y(y), _width(width), _height(height) {}
    /**
     * Getters
     */
    uint32_t getX() const { return _x; }
    uint32_t getY() const { return _y; }
    uint32_t getWidth() const { return _width; }
    uint32_t getHeight() const { return _height; }
    /**
     * Setters
     */
    void getX(uint32_t x) { x = _x; }
    void getY(uint32_t y) { y = _y; }
    void getWidth(uint32_t width) { width = _width; }
    void getHeight(uint32_t height) { height = _height; }
  private:
    uint32_t _x;      /**< X origin of the viewport in the window */
    uint32_t _y;      /**< Y origin of the viewport in the window */
    uint32_t _width;  /**< Width of the viewport */
    uint32_t _height; /**< Height of the viewport */
};
