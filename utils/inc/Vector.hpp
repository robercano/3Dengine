/**
 * @class	Vector
 * @brief	Manual vector3 representation to take care of data packing
 *          so serialization of a model to/from disk is faster
 */
#pragma once

#include "glm/glm.hpp"

struct Vec3
{
    Vec3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vec3(const Vec3 &v);
    Vec3(const glm::vec3 &glmv);
    Vec3 &operator=(const glm::vec3 &glmv);
    bool operator!=(const glm::vec3 &glmv);
    Vec3 &operator+=(const glm::vec3 &glmv);
    Vec3 operator-(const Vec3 &b);
    Vec3 &operator-=(const glm::vec3 &glmv);
    Vec3 &operator/=(float n);
    operator glm::vec3();
    operator glm::vec3() const;

    float x, y, z;
};

struct Vec2
{
    Vec2() : x(0.0f), y(0.0f){}
    Vec2(const Vec2 &v);
    Vec2(const glm::vec2 &glmv);
    Vec2 &operator=(const glm::vec2 &glmv);
    bool operator!=(const glm::vec2 &glmv);
    Vec2 &operator+=(const glm::vec2 &glmv);
    Vec2 operator-(const Vec2 &b);
    Vec2 &operator-=(const glm::vec2 &glmv);
    Vec2 &operator/=(float n);
    operator glm::vec2();
    operator glm::vec2() const;

    float x, y;
};

