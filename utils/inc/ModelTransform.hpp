/**
 * @class ModelTransform
 * @brief Transforms the vertex data of a Model3D
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <glm/glm.hpp>
#include "Model3D.hpp"

class ModelTransform
{
  public:
      /**
       * Rotates the vertices of the model using the given angles
       *
       * @param model        The model to be rotated
       * @param eulerAngles  Euler angles for each axis to be used for rotation
       */
      static void Rotate(Model3D &model, const glm::vec3 eulerAngles);

      /**
       * Translates the vertices of the model using the given offsets
       *
       * @param model    The model to be translated
       * @param offsets  Offsets for each coordinate
       */
      static void Translate(Model3D &model, const glm::vec3 offsets);

      /**
       * Appends one model to other and recalculates the indices, the offsets
       * and the indices count
       *
       * @param to    Model to append the data to
       * @param from  Model where the data is taken from
       */
      static void Append(Model3D &to, const Model3D &from);

      /**
       * Recalculates all the normals in the model by using the faces
       * and vertices information
       *
       * @param model  Model whose normals will be recalculated
       */
      static void RecalculateNormals(Model3D &model);
};
