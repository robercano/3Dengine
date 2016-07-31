/**
 * @class Asset3DTransform
 * @brief Transforms the vertex data of a Asset3D
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <glm/glm.hpp>
#include "Asset3D.hpp"

class Asset3DTransform
{
  public:
      /**
       * Rotates the vertices of the asset using the given angles
       *
       * @param asset        The asset to be rotated
       * @param eulerAngles  Euler angles for each axis to be used for rotation
       */
      static void Rotate(Asset3D &asset, const glm::vec3 eulerAngles);

      /**
       * Translates the vertices of the asset using the given offsets
       *
       * @param asset    The asset to be translated
       * @param offsets  Offsets for each coordinate
       */
      static void Translate(Asset3D &asset, const glm::vec3 offsets);

      /**
       * Appends one asset to other and recalculates the indices, the offsets
       * and the indices count
       *
       * @param to            Model to append the data to
       * @param from          Model where the data is taken from
       */
      static void Append(Asset3D &to, const Asset3D &from);

      /**
       * Appends one asset geometry to other and recalculates the indices, the offsets
       * and the indices count. The materials are not copied over. Typically used
       * to construct geometry without generating materials. At the end SetOnlyMaterial
       * can be used to assign a single material to the generated asset
       *
       * @param to            Model to append the data to
       * @param from          Model where the data is taken from
       */
      static void AppendGeometryOnly(Asset3D &to, const Asset3D &from);

      /**
       * Sets the given material as the only material in the asset, updating
       * the associated arrays so all the geometry is rendered with the given
       * material and texture
       *
       * @param asset     Model to add the material to
       * @param material  Material to be added
       * @param texture   Texture associated to the material
       */
      static void SetUniqueMaterial(Asset3D &asset, const Material &material, const Texture &texture);

      /**
       * Creates a material from the given color and then calls SetUniqueMaterial
       * with the created material
       *
       * @see SetUniqueMaterial
       *
       * @param asset  Model to add the material to
       * @param color  Color to use for the material creation
       */
      static void SetUniqueMaterialFromColor(Asset3D &asset, const glm::vec3 &color);

      /**
       * Recalculates all the normals in the asset by using the faces
       * and vertices information
       *
       * @param asset  Model whose normals will be recalculated
       */
      static void RecalculateNormals(Asset3D &asset);
};
