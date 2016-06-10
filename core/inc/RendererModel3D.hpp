/**
 * @class	RendererModel3D
 * @brief	Object 3D representation prepared for the specific renderer
 *          This is a composite model generated through Renderer::prepareModel3D()
 *          that prepares the model for the specific renderer. For example in
 *          OpenGL it would generate the VBO specific to the model 3D and cache
 *          that value so it can be used later on
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <glm/glm.hpp>
#include "Model3D.hpp"
#include "Object3D.hpp"

class RendererModel3D : public Object3D
{
  public:
    virtual ~RendererModel3D() {}
};
