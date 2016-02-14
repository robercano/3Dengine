/**
 * @class OBJLoader
 * @brief OBJ format loader
 *
 * @author Roberto Cano
 */
#pragma once

#include "Object3D.hpp"
#include <string>

class OBJLoader
{
    public:
        /**
         * Static method that given a .obj filename loads its contents
         * into a Object3D object that can then be rendered into the scene
         *
         * @param filename  Name of the file with the OBJ data
         * @param object    Output pointer to the Object3D object
         *
         * @return true if the object could be loaded, false otherwise
         */
        static bool Load(const std::string &filename, Object3D *& object);
};
