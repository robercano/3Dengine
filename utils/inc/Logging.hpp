/**
 * @file    Logging.hpp
 * @brief	Logging facilities
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <stdio.h>
#include <glm/glm.hpp>

class Model3D;
class Asset3D;

namespace Logging
{
    /**
     * Various logging facilities for different complex types
     */
    void log(const char *msg, const glm::mat4 &matrix);
    void log(const char *msg, const glm::mat3 &matrix);
    void log(const char *msg, const glm::mat2 &matrix);
    void log(const char *msg, const glm::vec4 &matrix);
    void log(const char *msg, const glm::vec3 &matrix);
    void log(const char *msg, const glm::vec2 &matrix);
    void log(const char *msg, const Model3D &model);
    void log(const char *msg, const Asset3D &asset);

    /**
     * Main logging function
     */
#if defined(_WIN32) || defined(_WIN64)
	void log(const char *format, ...);
#else
    void log(const char *format, ...) __attribute__((format(printf, 1, 2)));
#endif // _WIN32 || _WIN64
};
