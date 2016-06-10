/**
 * @class	ToonRenderTarget
 * @brief	A render target allows to render objects to it instead of
 *          to the main screen. The target can then be blitted onto the
 *          screen
 *
 *			The Toon render target post processed a model to add cartoon
 *			borders to the edges
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <glm/glm.hpp>
#include "RenderTarget.hpp"

class ToonRenderTarget : public virtual RenderTarget
{
  public:
    static ToonRenderTarget *New();
    static void Delete(ToonRenderTarget *target);

    void setNear(float near) { _near = near; }
    void setFar(float far) { _far = far; }
    void setBorderColor(const glm::vec4 &color) { _color = color; }
  protected:
    ToonRenderTarget() : _near(0.1f), _far(1000.0f), _color(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) {}
    virtual ~ToonRenderTarget() {}
    float _near;
    float _far;
    glm::vec4 _color;
};
