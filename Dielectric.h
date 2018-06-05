//
// Created by etienne cella on 2018-05-20.
//

#pragma once
#include "Structs.h"

inline float schlick(float cosine, float refractionIndex)
{
    const auto r0 = (1.0f - refractionIndex) / (1.0f + refractionIndex);
    return r0 * r0 + (1.0f - r0 * r0) * pow(1.0f - cosine, 5.0f);
}

struct Dielectric
{
    Dielectric(float refractionIndex_) : refractionIndex(refractionIndex_) { }
    float refractionIndex;
};

struct DielectricShader
{
    void operator()(const Buffer& shadingBuffer, std::vector<Ray>& rayBounced, std::vector<Ray>& rayAbsorbed)
    {
        ConstVectorView<RayShadingData<Dielectric>> shading(&shadingBuffer);

        for (size_t i = 0; i != shading.size(); ++i)
        {
            const auto& s = shading[i];
            Vec3 outwardNormal;
            const auto reflected = Vec3::reflect(s.ray.direction, s.hit.normal);
            float niOverNt;
            float cosine;

            if (Vec3::dot(s.ray.direction, s.hit.normal) > 0)
            {
                outwardNormal = s.hit.normal * -1;
                niOverNt = s.material.refractionIndex;
                cosine = s.material.refractionIndex * Vec3::dot(s.ray.direction, s.hit.normal) / s.ray.direction.length();
            }
            else
            {
                outwardNormal = s.hit.normal;
                niOverNt = 1.0f / s.material.refractionIndex;
                cosine = -Vec3::dot(s.ray.direction, s.hit.normal) / s.ray.direction.length();
            }

            Vec3 refracted;
            const auto refraction = Vec3::refract(s.ray.direction, outwardNormal, niOverNt, refracted);
            const auto reflectedProb = refraction ? schlick(cosine, s.material.refractionIndex) : 1.0f;
            rayBounced.push_back(Ray(s.hit.point, rand01() < reflectedProb ? reflected : refracted, s.ray.color, s.ray.index));
        }
    }
};
