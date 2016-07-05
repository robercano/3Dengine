/**
 * @file    Logging.cpp
 * @brief	Logging facilities
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include "Logging.hpp"
#include <stdarg.h>

void Logging::log(const char *msg, const glm::mat4 &matrix)
{
    printf("[mat4] %s\n    [%f, %f, %f, %f]\n    [%f, %f, %f, %f]\n    [%f, %f, %f, %f]\n    [%f, %f, %f, %f]\n", msg, matrix[0][0],
           matrix[1][0], matrix[2][0], matrix[3][0], matrix[0][1], matrix[1][1], matrix[2][1], matrix[3][1], matrix[0][2], matrix[1][2],
           matrix[2][2], matrix[3][2], matrix[0][3], matrix[1][3], matrix[2][3], matrix[3][3]);
}

void Logging::log(const char *msg, const glm::mat3 &matrix)
{
    printf("[mat3] %s\n    [%f, %f, %f]\n    [%f, %f, %f]\n    [%f, %f, %f]\n", msg, matrix[0][0], matrix[1][0], matrix[2][0], matrix[0][1],
           matrix[1][1], matrix[2][1], matrix[0][2], matrix[1][2], matrix[2][2]);
}

void Logging::log(const char *msg, const glm::mat2 &matrix)
{
    printf("[mat3] %s\n    [%f, %f]\n    [%f, %f]\n", msg, matrix[0][0], matrix[1][0], matrix[0][1], matrix[1][1]);
}

void Logging::log(const char *msg, const glm::vec4 &vect)
{
    printf("[vec4] %s\n    [%f, %f, %f, %f]\n", msg, vect[0], vect[1], vect[2], vect[3]);
}

void Logging::log(const char *msg, const glm::vec3 &vect) { printf("[vec3] %s\n    [%f, %f, %f]\n", msg, vect[0], vect[1], vect[2]); }
void Logging::log(const char *msg, const glm::vec2 &vect) { printf("[vec2] %s\n    [%f, %f]\n", msg, vect[0], vect[1]); }
void Logging::log(const char *msg, const Model3D &model)
{
    uint32_t i;

    printf("[Model3D] %s\n", msg);

    printf("  [Vertices] %lu\n", model.getVertexData().size());
    i = 0;
    for (std::vector<Model3D::VertexData>::const_iterator it = model.getVertexData().begin(); it != model.getVertexData().end();
         ++it, ++i) {
        printf("    [%05d] vertex: [%f, %f, %f]\n", i, it->vertex.x, it->vertex.y, it->vertex.z);
        printf("            normal: [%f, %f, %f]\n", it->normal.x, it->normal.y, it->normal.z);
        printf("            uvcoord:[%f, %f]\n", it->uvcoord.x, it->uvcoord.y);
    }

    printf("  [Indices] %lu\n", model.getIndexData().size());
    i = 0;
    for (std::vector<uint32_t>::const_iterator it = model.getIndexData().begin(); it != model.getIndexData().end(); ++it, ++i) {
        printf("    [%05d] index: %d\n", i, *it);
    }

    printf("  [Materials] %lu\n", model.getMaterials().size());
    for (i = 0; i < model.getMaterials().size(); ++i) {
        printf("    [%05d]\t material:  Ka [%f, %f, %f]\n", i, model.getMaterials()[i].getAmbient().r,
               model.getMaterials()[i].getAmbient().g, model.getMaterials()[i].getAmbient().b);
        printf("           \t            Kd [%f, %f, %f]\n", model.getMaterials()[i].getDiffuse().r, model.getMaterials()[i].getDiffuse().g,
               model.getMaterials()[i].getDiffuse().b);
        printf("           \t            Ks [%f, %f, %f]\n", model.getMaterials()[i].getSpecular().r,
               model.getMaterials()[i].getSpecular().g, model.getMaterials()[i].getSpecular().b);
        printf("           \t            alpha [%f]\n", model.getMaterials()[i].getAlpha());
        printf("           \t            shine [%f]\n", model.getMaterials()[i].getShininess());
        printf("           \t offset: %d\n", model.getIndicesOffsets()[i]);
        printf("           \t count:  %d\n", model.getIndicesCount()[i]);
    }
}

void Logging::log(const char *format, ...)
{
    va_list arglist;

    va_start(arglist, format);
    vprintf(format, arglist);
    va_end(arglist);
}
