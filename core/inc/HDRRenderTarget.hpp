/**
 * @class	HDRRenderTarget
 * @brief	Render target that supports HDR rendering and resolves to the main
 *          framebuffer by applying tone mapping
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "RenderTarget.hpp"

class HDRRenderTarget : public virtual RenderTarget
{
  public:
    /**
     * Allocates a new render target using the underlaying rendering API
     *
     * @return Allocated HDR render target or NULL if OOM
     */
    static HDRRenderTarget *New();

    /**
     * Deletes an HDR render target previously allocated with New()
     *
     * @param The render target pointer
     */
    static void Delete(HDRRenderTarget *target);

    /**
     * Destructor
     */
    virtual ~HDRRenderTarget() {}
    /**
     * Sets the exposure level for the tone mapping
     *
     * @param exposure  Exposure level
     */
    void setExposure(float exposure) { _exposure = exposure; }
    /**
     * Gets the exposure level for the tone mapping
     *
     * @return Exposure level
     */
    float getExposure() { return _exposure; }

    /**
     * Disables tone mapping when blitting the target
     */
    void disableToneMapping() { _toneMapping = false; }

    /**
     * Enables tone mapping when blitting the target
     */
    void enableToneMapping() { _toneMapping = true; }

  protected:
    /**
     * Constructor
     */
    HDRRenderTarget() : _exposure(1.0f), _toneMapping(true) {}

    float _exposure;   /**< Exposure level used for tone mapping */
    bool _toneMapping; /**< Indicates if we must do tone mapping */
};
