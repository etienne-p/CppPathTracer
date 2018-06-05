//
// Created by etienne cella on 2018-05-19.
//

#pragma once

#include <vector>
#include <cassert>
#include "Structs.h"

struct Camera
{
    Camera(
            const Vec3& lookFrom,
            const Vec3& lookAt,
            const Vec3& vUp,
            float verticalFov,
            float aspect,
            float aperture,
            float focusDist)
    {
        lensRadius = aperture * 0.5f;
        const auto theta = verticalFov * M_PI / 180.0f;
        const auto halfHeight = tan(theta * 0.5f);
        const auto halfWidth = aspect * halfHeight;
        origin = lookFrom;
        w = Vec3::normalize(lookFrom - lookAt);
        u = Vec3::normalize(Vec3::cross(vUp, w));
        v = Vec3::cross(w, u);
        lowerLeftCorner = origin - u * halfWidth * focusDist - v * halfHeight * focusDist - w * focusDist;
        horizontal = u * 2 * halfWidth * focusDist;
        vertical = v * 2 * halfHeight * focusDist;
    }

    void initializeRays(std::vector<Ray>& rays, int resolutionWidth, int resolutionHeight, int antialiasing) const
    {
        assert(rays.size() == 0);
        for (int y = 0; y != resolutionHeight; ++y)
        {
            for (int x = 0; x != resolutionWidth; ++x)
            {
                for (int k = 0; k != antialiasing; ++k)
                {
                    const auto u = (x + rand01()) / float(resolutionWidth - 1);
                    const auto v = (y + rand01()) / float(resolutionHeight - 1);
                    const auto rnd = Vec3::randomInUnitDisk() * lensRadius;
                    const auto offset = u * rnd.x() + v * rnd.y();
                    rays.push_back(Ray(
                            origin + offset,
                            lowerLeftCorner + horizontal * u + vertical * v - origin - offset,
                            Vec3::one(), y * resolutionWidth + x));
                }
            }
        }
    }

    Vec3 lowerLeftCorner, horizontal, vertical, origin, u, v, w;
    float lensRadius;
};
