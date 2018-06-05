//
// Created by etienne cella on 2018-05-19.
//

#pragma once
#include <vector>
#include "Structs.h"
#include "Buffer.h"

struct Metal
{
    Metal(Vec3 albedo_, float fuzziness_) : albedo(albedo_), fuzziness(fuzziness_) { }

    Vec3 albedo;
    float fuzziness;
};

struct MetalShader
{
    void operator()(const Buffer& shadingBuffer, std::vector<Ray>& rayBounced, std::vector<Ray>& rayAbsorbed)
    {
        ConstVectorView<RayShadingData<Metal>> shading(&shadingBuffer);
        for (size_t i = 0; i != shading.size(); ++i)
        {
            const auto& s = shading[i];
            const auto reflected = Vec3::reflect(Vec3::normalize(s.ray.direction), s.hit.normal);
            const auto scatteredDir = reflected + Vec3::randomInUnitSphere() * s.material.fuzziness;
            if(Vec3::dot(scatteredDir, s.hit.normal))
            {
                rayBounced.push_back(Ray(
                        s.hit.point, scatteredDir,
                        s.ray.color * s.material.albedo, s.ray.index));
            }
            else
            {
                auto r = s.ray;
                r.color *= 0;
                rayAbsorbed.push_back(r);
            }
        }
    }
};