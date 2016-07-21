/**
 * @class	Light
 * @brief   Light object
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Object3D.hpp"
#include "Projection.hpp"
#include "ShadowMapRenderTarget.hpp"

/**
 * Light inherits from Camera to be able to implement Shadow maps by projecting the models
 * into the light as if it were a camera. Need to check if there is a better hierarchy for this */
class Light : public Projection, public Object3D
{
  public:
    /* TODO: add comments */
    Light(const glm::vec3 &ambient = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3 &diffuse = glm::vec3(0.0f, 0.0f, 0.0f),
          const glm::vec3 &specular = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3 &position = glm::vec3(0.0f, 0.0f, 0.0f))
        : _ambient(ambient), _diffuse(diffuse), _specular(specular)
    {
        _shadowMap = ShadowMapRenderTarget::New();
        setPosition(position);
    }
    ~Light() { ShadowMapRenderTarget::Delete(_shadowMap); }
    void setAmbient(const glm::vec3 &ambient) { _ambient = ambient; }
    void setDiffuse(const glm::vec3 &diffuse) { _diffuse = diffuse; }
    void setSpecular(const glm::vec3 &specular) { _specular = specular; }
    glm::vec3 getAmbient() { return _ambient; }
    glm::vec3 getDiffuse() { return _diffuse; }
    glm::vec3 getSpecular() { return _specular; }
    RenderTarget *getShadowMap() { return _shadowMap; }
    virtual const glm::mat4 &getProjectionMatrix() = 0;

    /**
     * Debug information
     */
    void setRenderMarker(bool flag) { _renderMarker = flag; }
    bool getRenderMarker(void) { return _renderMarker; }

  protected:
    glm::vec3 _ambient;
    glm::vec3 _diffuse;
    glm::vec3 _specular;
    ShadowMapRenderTarget *_shadowMap;
    bool _renderMarker;
};
