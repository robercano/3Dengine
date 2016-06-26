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
    void log(const char *msg, const glm::mat4 &matrix);
    void log(const char *msg, const glm::mat3 &matrix);
};
