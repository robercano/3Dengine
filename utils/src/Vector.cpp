/**
 * @class	Vector
 * @brief	Manual vector3 representation to take care of data packing
 *          so serialization of a model to/from disk is faster
 */
#include "Vector.hpp"

Vec3::Vec3(const glm::vec3 &glmv)
{
    x = glmv.x;
    y = glmv.y;
    z = glmv.z;
}
Vec3::Vec3(const Vec3 &v)
{
    x = v.x;
    y = v.y;
    z = v.z;
}
Vec3 &Vec3::operator=(const glm::vec3 &glmv)
{
    x = glmv.x;
    y = glmv.y;
    z = glmv.z;

    return *this;
}
bool Vec3::operator!=(const glm::vec3 &glmv) { return x != glmv.x || y != glmv.y || z != glmv.z; }
Vec3::operator glm::vec3() { return glm::vec3(x, y, z); }
Vec3::operator glm::vec3() const { return glm::vec3(x, y, z); }
Vec3 &Vec3::operator+=(const glm::vec3 &glmv)
{
    x += glmv.x;
    y += glmv.y;
    z += glmv.z;

    return *this;
}
Vec3 Vec3::operator-(const Vec3 &b)
{
    Vec3 v(*this);

    v.x -= b.x;
    v.y -= b.y;
    v.z -= b.z;

    return v;
}
Vec3 &Vec3::operator-=(const glm::vec3 &glmv)
{
    x -= glmv.x;
    y -= glmv.y;
    z -= glmv.z;

    return *this;
}
Vec3 &Vec3::operator/=(float n)
{
    x /= n;
    y /= n;
    z /= n;

    return *this;
}

Vec2::Vec2(const glm::vec2 &glmv)
{
    x = glmv.x;
    y = glmv.y;
}
Vec2::Vec2(const Vec2 &v)
{
    x = v.x;
    y = v.y;
}
Vec2 &Vec2::operator=(const glm::vec2 &glmv)
{
    x = glmv.x;
    y = glmv.y;

    return *this;
}
bool Vec2::operator!=(const glm::vec2 &glmv) { return x != glmv.x || y != glmv.y; }
Vec2::operator glm::vec2() { return glm::vec2(x, y); }
Vec2::operator glm::vec2() const { return glm::vec2(x, y); }
Vec2 &Vec2::operator+=(const glm::vec2 &glmv)
{
    x += glmv.x;
    y += glmv.y;

    return *this;
}
Vec2 Vec2::operator-(const Vec2 &b)
{
    Vec2 v(*this);

    v.x -= b.x;
    v.y -= b.y;

    return v;
}
Vec2 &Vec2::operator-=(const glm::vec2 &glmv)
{
    x -= glmv.x;
    y -= glmv.y;

    return *this;
}
Vec2 &Vec2::operator/=(float n)
{
    x /= n;
    y /= n;

    return *this;
}
