/**
 * @file    Logging.hpp
 * @brief	Logging facilities
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <stdio.h>
#include <glm/glm.hpp>

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

    /**
     * Main logging function
     */
    void log(const char *format, ...) __attribute__((format(printf, 1, 2)));
};
