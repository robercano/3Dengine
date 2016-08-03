/**
 * @class	PointLight
 * @brief   This type of light is defined by a point in space and radiates in a spherical
 *          manner. It can have attenuation.
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <glm/glm.hpp>
#include "Light.hpp"

class PointLight : public Light
{
  public:
      /**
       * Constructor
       *
       * @param ambient     Ambient component color
       * @param diffuse     Diffuse component color
       * @param specular    Specular component color
       * @param position    Position in world coordinates of this light
       * @param attenuation Attenuation factor to be used for power attenuation
       * @param cutoff      Cutoff distance from the position of the light where
       *                    the light does not affect the  geometry anymore
       */
      PointLight(const glm::vec3 &ambient = glm::vec3(1.0f, 1.0f, 1.0f), const glm::vec3 &diffuse = glm::vec3(1.0f, 1.0f, 1.0f),
              const glm::vec3 &specular = glm::vec3(1.0f, 1.0f, 1.0f), const glm::vec3 &position = glm::vec3(0.0f, 0.0f, 0.0f),
              float attenuation = 1.0f, float cutoff = 1e6f)
          : Light(ambient, diffuse, specular, position), _attenuation(attenuation), _cutoff(cutoff) { }


      /**
       * Destructor
       */
      ~PointLight() {}

      /**
       * Setters
       */
      void setAttenuation(float attenuation) { _attenuation = attenuation; }
      void setCutoff(float cutoff) { _cutoff = cutoff; }

      /**
       * Getters
       */
      float getAttenuation() { return _attenuation; }
      float getCutoff() { return _cutoff; }

      /**
       * Gets the projection matrix for this light to be used for
       * shadow mapping
       *
       * @return The 4x4 projection matrix used to shadow map onto this light
       */
      const glm::mat4 &getProjectionMatrix() { return getPerspectiveMatrix(); }

      /**
       * Inherited from Object3D to calculate the bounding volumes of this light
       * based on its position and cutoff factor
       */
      void _calculateBoundingVolumes()
      {
          float cutoff = _cutoff / glm::sqrt(3.0f);

          _maxLengthVertex = glm::vec3(cutoff, cutoff, cutoff);

          _oobb.setMin(-_maxLengthVertex);
          _oobb.setMax(_maxLengthVertex);

          _updateBoundingVolumes();
      }

  protected:
      float _attenuation; /**< Attenuation factor of this light used for power attenuation */
      float _cutoff;      /**< Cutoff factor to determine at which distance the light does
                               not affect geometry anymore */
};
