/**
 * @class ModelInfo
 * @brief Takes care of saving an existing in-memory model to disk and viceversa
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include "ModelInfo.hpp"
#include "Logging.hpp"

using namespace Logging;

void ModelInfo::ShowModelInfo(Model3D &model)
{
    log("Vertices:  %lu\n", model.getVertexData().size());
    log("Materials: %lu\n", model.getMaterials().size());
    for (uint32_t i = 0; i < model.getMaterials().size(); ++i) {
        log("  Mat. %d\n", i);
        log("    Ambient:  (%f, %f, %f)\n", model.getMaterials()[i].getAmbient().r, model.getMaterials()[i].getAmbient().g,
            model.getMaterials()[i].getAmbient().b);
        log("    Diffuse:  (%f, %f, %f)\n", model.getMaterials()[i].getDiffuse().r, model.getMaterials()[i].getDiffuse().g,
            model.getMaterials()[i].getDiffuse().b);
        log("    Specular: (%f, %f, %f)\n", model.getMaterials()[i].getSpecular().r, model.getMaterials()[i].getSpecular().g,
            model.getMaterials()[i].getSpecular().b);
        log("    Alpha:    %f\n", model.getMaterials()[i].getAlpha());
        log("    Shine:    %f\n", model.getMaterials()[i].getShininess());
    }
    log("Textures:  %lu\n", model.getTextures().size());
    for (uint32_t i = 0; i < model.getTextures().size(); ++i) {
        log("  Tex. %d: [%d x %d x %d]\n", i, model.getTextures()[i]._width, model.getTextures()[i]._height, model.getTextures()[i]._Bpp);
    }
    log("Indices:   %lu\n", model.getIndexData().size());
    log("Offsets:   %lu\n", model.getIndicesOffsets().size());
    log("Counts:    %lu\n\n", model.getIndicesCount().size());
}
