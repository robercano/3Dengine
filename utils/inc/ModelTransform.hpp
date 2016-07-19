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
       * @param to            Model to append the data to
       * @param from          Model where the data is taken from
       */
      static void Append(Model3D &to, const Model3D &from);

      /**
       * Appends one model geometry to other and recalculates the indices, the offsets
       * and the indices count. The materials are not copied over. Typically used
       * to construct geometry without generating materials. At the end SetOnlyMaterial
       * can be used to assign a single material to the generated model
       *
       * @param to            Model to append the data to
       * @param from          Model where the data is taken from
       */
      static void AppendGeometryOnly(Model3D &to, const Model3D &from);

      /**
       * Sets the given material as the only material in the model, updating
       * the associated arrays so all the geometry is rendered with the given
       * material and texture
       *
       * @param model     Model to add the material to
       * @param material  Material to be added
       * @param texture   Texture associated to the material
       */
      static void SetUniqueMaterial(Model3D &model, const Material &material, const Texture &texture);

      /**
       * Creates a material from the given color and then calls SetUniqueMaterial
       * with the created material
       *
       * @see SetUniqueMaterial
       *
       * @param model  Model to add the material to
       * @param color  Color to use for the material creation
       */
      static void SetUniqueMaterialFromColor(Model3D &model, const glm::vec3 &color);

      /**
       * Recalculates all the normals in the model by using the faces
       * and vertices information
       *
       * @param model  Model whose normals will be recalculated
       */
      static void RecalculateNormals(Model3D &model);
};
