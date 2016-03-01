/**
 * @class	RendererObject3D
 * @brief	Object 3D representation prepared for the specific renderer
 *          This is a composite object generated through Renderer::prepareObject3D()
 *          that prepares the object for the specific renderer. For example in
 *          OpenGL it would generate the VBO specific to the object 3D and cache
 *          that value so it can be used later on
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "Object3D.hpp"

class RendererObject3D
{
	public:
        virtual ~RendererObject3D() {}
};
