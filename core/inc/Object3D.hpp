/**
 * @class	Object3D
 * @brief	Basic object 3D representation
 *
 * @author	Roberto Sosa Cano
 */
#pragma once

#include <stdint.h>

class Object3D
{
	public:
		/**
		 * Destructor
		 */
		virtual ~Object3D() {}

        /**
         * Returns the vertices array information
         */
        virtual const float *getVerticesArray() const = 0;
        virtual uint32_t getVerticesArrayLen() const = 0;

        /**
         * Returns the colors array information
         */
        virtual const float *getColorsArray() const = 0;
        virtual uint32_t getColorsArrayLen() const = 0;

        /**
         * Returns the normals array information
         */
        virtual const float *getNormalsArray() const = 0;
        virtual uint32_t getNormalsArrayLen() const = 0;

        /**
         * Returns the colors array information
         */
        virtual const uint32_t *getIndicesArray() const = 0;
        virtual uint32_t getIndicesArrayLen() const = 0;
};
