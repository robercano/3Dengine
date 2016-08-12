/**
 * @class	SolidColoringShader
 * @brief   Outputs all fragments of the shader with the
 *          given color
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <glm/glm.hpp>

class SolidColoringShader
{
  public:
    static SolidColoringShader *New();
    static void Delete(SolidColoringShader *target);

    /**
     * Sets the color for all output fragments
     *
     * @param color  RGBA color
     */
    void setColor(const glm::vec4 &color) { _color = color; }

    /**
     * Gets the color for all output fragments
     *
     * @return RGBA color used for all fragments
     */
    const glm::vec4 &getColor() { return _color; }

  protected:
    glm::vec4 _color;  /**< Color used for all output fragments */
};
