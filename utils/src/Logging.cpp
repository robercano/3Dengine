/**
 * @file    Logging.cpp
 * @brief	Logging facilities
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include "Logging.hpp"

void Logging::log(const char *msg, const glm::mat4 &matrix)
{
    printf("[mat4] %s\n    [%f, %f, %f, %f]\n    [%f, %f, %f, %f]\n    [%f, %f, %f, %f]\n    [%f, %f, %f, %f]\n",
            msg,
            matrix[0][0], matrix[1][0], matrix[2][0], matrix[3][0],
            matrix[0][1], matrix[1][1], matrix[2][1], matrix[3][1],
            matrix[0][2], matrix[1][2], matrix[2][2], matrix[3][2],
            matrix[0][3], matrix[1][3], matrix[2][3], matrix[3][3]);
}

void Logging::log(const char *msg, const glm::mat3 &matrix)
{
    printf("[mat3] %s\n    [%f, %f, %f]\n    [%f, %f, %f]\n    [%f, %f, %f]\n",
            msg,
            matrix[0][0], matrix[1][0], matrix[2][0],
            matrix[0][1], matrix[1][1], matrix[2][1],
            matrix[0][2], matrix[1][2], matrix[2][2]);
}
