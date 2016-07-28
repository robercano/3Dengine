/**
 * @class ModelStorage
 * @brief Takes care of saving an existing in-memory model to disk and viceversa
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <string>
#include "Model3D.hpp"

class ModelStorage
{
  public:
      /**
       * Saves a Model3D to disk with the given name
       *
       * @param name   Name of the model
       * @param model  Model to be saved to disk
       *
       * @return true if the model was saved correctly or false
       *         otherwise
       */
      static bool Save(const std::string &name, const Model3D &model);

      /**
       * Loads a Model3D from disk with the given name
       *
       * @param name   Name of the model
       * @param model  Model to be loaded to disk
       *
       * @return true if the model was loaded correctly or false
       *         otherwise
       */
      static bool Load(const std::string &name, Model3D &model);
};
