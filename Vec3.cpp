//
// Created by etienne cella on 2018-04-23.
//

#include "Vec3.h"

bool Vec3::refract(const Vec3& v, const Vec3& n, float niOverNt, Vec3& refracted)
{
    const auto vNorm = Vec3::normalize(v);
    const auto dt = Vec3::dot(vNorm, n);
    const auto discriminant = 1.0f - niOverNt * niOverNt * (1.0f - dt * dt);
    if (discriminant > 0)
    {
        refracted = (vNorm - n * dt) * niOverNt - n * sqrtf(discriminant);
        return true;
    }
    return false;
}

Vec3 Vec3::randomInUnitSphere()
{
    // implemented using a rejection method
    Vec3 p;
    do
    {
        p = Vec3(rand01(), rand01(), rand01()) * 2.0f - 1.0f;
    }
    while(p.squaredLength() >= 1.0f);
    return p;
}

float Vec3::dot(const Vec3& v1, const Vec3& v2)
{
    return (v1.v[0] * v2.v[0]) + (v1.v[1] * v2.v[1]) + (v1.v[2] *  v2.v[2]);
}

Vec3 Vec3::lerp(const Vec3& v1, const Vec3& v2, float t)
{
    return v1 * (1 - t) + v2 * t;
}

Vec3 Vec3::cross(const Vec3& v1, const Vec3& v2)
{
    return Vec3(
            v1.v[1] * v2.v[2] - v1.v[2] * v2.v[1],
            v1.v[2] * v2.v[0] - v1.v[0] * v2.v[2],
            v1.v[0] * v2.v[1] - v1.v[1] * v2.v[0]);
}

Vec3 Vec3::normalize(const Vec3& v)
{
    return v / v.length();
}

Vec3 Vec3::reflect(const Vec3& v, const Vec3& n)
{
    return v - n * 2.0f * Vec3::dot(v, n);
}

Vec3 Vec3::randomInUnitDisk()
{
    // rejection method
    Vec3 p;
    do
    {
        p = Vec3(rand01(), rand01(), 0) * 2 - Vec3(1, 1, 0);
    }
    while(Vec3::dot(p, p) >= 1.0f);
    return p;
}

