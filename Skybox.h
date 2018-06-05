//
// Created by etienne cella on 2018-05-19.
//

#pragma once
#include "Structs.h"

struct Skybox
{
    Vec3 operator()(const Ray &ray) const
    {
        const auto normDir = Vec3::normalize(ray.direction);
        const auto t = 0.5f * (1.0f + normDir.y());
        return Vec3::lerp(Vec3::one(), Vec3(0.5f, 0.7f, 1.0f), t);
    }
};