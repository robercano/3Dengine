/**
 * @class	Object3D
 * @brief	Basic object 3D representation
 *
 * @author	Roberto Sosa Cano
 */
#pragma once

#include <stdint.h>
#include <glm/glm.hpp>

class Object3D
{
	public:
        typedef struct {
            glm::vec3 vertex;
            glm::vec3 normal;
            glm::vec2 uvcoord;
        } VertexData;

		/**
		 * Destructor
		 */
		virtual ~Object3D() {}

        virtual const VertexData *getVertexData() const = 0;
        virtual uint32_t getVertexDataLen() const = 0;
        virtual uint32_t getVertexDataSize() const = 0;
        virtual const uint32_t *getIndicesArray() const = 0;
        virtual uint32_t getIndicesArrayLen() const = 0;
        virtual uint32_t getIndicesArraySize() const = 0;
};
