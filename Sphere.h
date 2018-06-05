//
// Created by etienne cella on 2018-05-19.
//

#pragma once

#include "Structs.h"
#include "Buffer.h"

struct Sphere
{
    Sphere(Vec3 center_, float radius_) : center(center_), radius(radius_) { }
    Vec3 center;
    float radius;
};

struct SphereCollider
{
    bool operator()(const Buffer& primitives, const Ray& ray, HitRecord& hit, int meshIndex) const
    {
        ConstVectorView<Sphere> spheres(&primitives);
        bool rv = false;
        for (size_t i = 0; i != spheres.size(); ++i)
        {
            const auto& sphere = spheres[i];
            const auto oc = ray.origin - sphere.center;
            const auto a = Vec3::dot(ray.direction, ray.direction);
            const auto b = Vec3::dot(oc, ray.direction);
            const auto c = Vec3::dot(oc, oc) - sphere.radius * sphere.radius;
            const auto discriminant = b * b - a * c;

            if (discriminant <= 0) continue;

            const auto discriminantSqrt = sqrt(discriminant);
            const auto tmp0 = (-b - discriminantSqrt) / a;
            const auto tmp1 = (-b + discriminantSqrt) / a;
            float tmp;

            constexpr auto EPSILON = 1e-4f;

            if (tmp0 < hit.t && tmp0 > EPSILON)
            {
                tmp = tmp0;
            }
            else if (tmp1 < hit.t && tmp1 > EPSILON)
            {
                tmp = tmp1;
            }
            else
            {
                continue;
            }

            hit.point = ray.pointAtParameter(tmp);
            hit.normal = (hit.point - sphere.center) / sphere.radius;
            hit.t = tmp;
            hit.meshIndex = meshIndex;
            hit.primitiveIndex = i;
            rv = true;
        }
        return rv;
    }
};
