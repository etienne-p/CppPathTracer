//
// Created by etienne cella on 2018-05-19.
//

#pragma once
#include <vector>
#include "Structs.h"
#include "Buffer.h"

struct Lambertian
{
    Lambertian(Vec3 albedo_) : albedo(albedo_) { }
    Vec3 albedo;
};

struct LambertianShader
{
    void operator()(const Buffer& shadingBuffer, std::vector<Ray>& rayBounced, std::vector<Ray>& rayAbsorbed)
    {
        ConstVectorView<RayShadingData<Lambertian>> shading(&shadingBuffer);
        for (size_t i = 0; i != shading.size(); ++i)
        {
            const auto& s = shading[i];
            const auto target = s.hit.point + s.hit.normal + Vec3::randomInUnitSphere();
            rayBounced.push_back(Ray(
                    s.hit.point, target - s.hit.point,
                    s.ray.color * s.material.albedo, s.ray.index));
        }
    }
};
