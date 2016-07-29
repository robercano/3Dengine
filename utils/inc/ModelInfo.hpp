/**
 * @class ModelInfo
 * @brief Takes care of saving an existing in-memory model to disk and viceversa
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <string>
#include "Model3D.hpp"

namespace ModelInfo
{
    /**
     * Prints information about the model
     *
     * @param model  Model to print information about
     */
    void ShowModelInfo(Model3D &model);
};
