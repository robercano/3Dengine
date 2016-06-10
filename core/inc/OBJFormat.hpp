/**
 * @class OBJLoader
 * @brief OBJ format loader
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <glm/glm.hpp>
#include <map>
#include <map>
#include <string>
#include <vector>

#include "Material.hpp"
#include "Model3D.hpp"
#include "Texture.hpp"

class OBJFormat : public Model3D
{
  public:
    /**
     * Method that given a .obj filename loads its contents
     * into a Model3D model that can then be rendered into the scene
     *
     * @param filename  Name of the file with the OBJ data
     *
     * @return true if the model could be loaded, false otherwise
     */
    bool load(const std::string &filename);
};
