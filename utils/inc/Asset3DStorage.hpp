/**
 * @class Asset3DStorage
 * @brief Takes care of saving an existing in-memory model to disk and viceversa
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <string>
#include "Asset3D.hpp"

class Asset3DStorage
{
  public:
    /**
     * Saves a Asset3D3D to disk with the given name
     *
     * @param name   Name of the model
     * @param model  Asset3D to be saved to disk
     *
     * @return true if the model was saved correctly or false
     *         otherwise
     */
    static bool Save(const std::string &name, const Asset3D &model);

    /**
     * Loads a Asset3D3D from disk with the given name
     *
     * @param name   Name of the model
     * @param model  Asset3D to be loaded to disk
     *
     * @return true if the model was loaded correctly or false
     *         otherwise
     */
    static bool Load(const std::string &name, Asset3D &model);
};
