/**
 * @class OBJLoader
 * @brief OBJ format loader. Supports a limited subset of the OBJ format. The
 *        model is given in the following directory structure:
 *
 *           <modelName>/
 *                 material.mtl
 *                 geometry.obj
 *
 *        material.mtl
 *        ------------
 *
 *        Contains the material definitions for the object material groups. Currently the
 *        supported parameters for a material are:
 *
 *            * Ka - Ambient component
 *            * Kd - Diffuse component
 *            * Ks - Specular component
 *            * Ns - Shininess
 *            * map_Kd - Diffuse map
 *
 *        No displacement-map or other features are supported.
 *
 *        geometry.obj
 *        ------------
 *
 *        Contains the geometry definition. There are 2 sections. A first
 *        section declaring the raw vertex data:
 *
 *            * v - Vertex 3D coordinate
 *            * vn - Vertex normal
 *            * vt - Vertex UV coordinate
 *
 *        And a second section containing the material groups:
 *
 *            * usemtl - Specifies the material from the first section to be used
 *            * f - Face specification, only triangles are supported. Each line specifies
 *                  the vertex coordinate, normal and UV coordinate for each of the 3 vertices
 *                  of the triangle
 *
 *        Once loaded the data is consolidated into single arrays for the model data so the
 *        renderer can loop per material, and then for each material a list of indexed display
 *        lists are provided.
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
