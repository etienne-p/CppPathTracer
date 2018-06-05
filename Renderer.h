//
// Created by etienne cella on 2018-06-02.
//

#pragma once

#include <map>
#include <algorithm>
#include "Structs.h"
#include "Buffer.h"
#include "World.h"
#include "Skybox.h"
#include "Metal.h"

constexpr auto EPSILON = 1e-4f;
constexpr int ABSORBED = 2;
constexpr int SKYBOX = 3;

using BufferMap = std::map<BufferType, std::unique_ptr<Buffer>>;
using RayBuffers = std::vector<std::unique_ptr<std::vector<Ray>>>;

inline void rasterize(const std::vector<Ray>& rays, std::vector<Vec3>& image)
{
    for (const auto& ray : rays)
    {
        image[ray.index] += ray.color;
    }
}

template <typename T>
inline void writeShadingEntry(const Buffer* materialBuffer, const Ray& ray, const HitRecord& hit, Buffer* shadingBuffer)
{
    StackView<RayShadingData<T>>(shadingBuffer)
            .push(RayShadingData<T>(ray, hit,
                                    ConstVectorView<T>(materialBuffer)[hit.primitiveIndex]));
}

inline void resetHitBuffer(std::vector<HitRecord>& hits)
{
    for (auto& hit : hits)
    {
        hit.t = std::numeric_limits<float>::max();
    }
}

inline void resetShadingBuffers(BufferMap& shadingData)
{
    for (auto &s : shadingData )
    {
        s.second->clear();
    }
}

inline void shade(const BufferMap& shadingBuffers, RayBuffers& rayData, int out)
{
    for (const auto& s : shadingBuffers)
    {
        switch(s.first)
        {
            case BufferType::lambertian:
                LambertianShader()(*s.second, *rayData[out], *rayData[ABSORBED]);
                break;
            case BufferType::metal:
                MetalShader()(*s.second, *rayData[out], *rayData[ABSORBED]);
                break;
            case BufferType::dielectric:
                DielectricShader()(*s.second, *rayData[out], *rayData[ABSORBED]);
                break;
            default:
                assert(0);
                break;
        }
    }
}

class Renderer
{
public:
    // note that the renderer is not supposed to alter the world and therefore gets passed
    // a pointer to a const object
    Renderer(const std::vector<Ray>& initRays_, std::shared_ptr<const World> world_)
    {
        world = world_;
        // create 4 ray buffers
        // - for double buffering
        // - for rays that do not intersect the scene
        // - for rays that got absorbed by the scene
        rayData.push_back(std::unique_ptr<std::vector<Ray>>(new std::vector<Ray>(initRays_)));
        for (auto i = 0; i != 3; ++i)
        {
            rayData.push_back(make_unique<std::vector<Ray>>());
            rayData.back()->reserve(initRays_.size());
        }
        // initialize a buffer for hit records, one per ray
        hits.resize(initRays_.size());
        // initialize shading buffers, one per material
        shadingBuffers.insert(std::make_pair(
                BufferType::lambertian,
                Buffer::create<RayShadingData<Lambertian>>(initRays_.size())));
        shadingBuffers.insert(std::make_pair(
                BufferType::metal,
                Buffer::create<RayShadingData<Metal>>(initRays_.size())));
        shadingBuffers.insert(std::make_pair(
                BufferType::dielectric,
                Buffer::create<RayShadingData<Dielectric>>(initRays_.size())));
    }

    void render(int maxDepth)
    {
        int in, iterations = 0;

        do
        {
            in = iterations % 2;
            out = (iterations + 1) % 2;
            // reset buffers
            resetHitBuffer(hits);
            resetShadingBuffers(shadingBuffers);
            rayData[out]->clear();
            // hit detection, scene traversal
            world->traverse(*rayData[in], hits);
            // populate shading buffers based on hit detection output
            for (size_t i = 0; i != hits.size(); ++i)
            {
                const auto &hit = hits[i];
                const auto &ray = (*rayData[in])[i];

                // if there is a hit
                if (hit.t + EPSILON < std::numeric_limits<float>::max())
                {
                    // enqueue shading data based on material
                    // TODO would sorting hits based on material type help?
                    switch ((*world)[hit.meshIndex].materialType)
                    {
                        case BufferType::lambertian :
                            writeShadingEntry<Lambertian>(
                                    &((*world)[hit.meshIndex].material),
                                    ray, hit, shadingBuffers[BufferType::lambertian].get());
                            break;
                        case BufferType::metal :
                            writeShadingEntry<Metal>(
                                    &((*world)[hit.meshIndex].material),
                                    ray, hit, shadingBuffers[BufferType::metal].get());
                            break;
                        case BufferType::dielectric :
                            writeShadingEntry<Dielectric>(
                                    &((*world)[hit.meshIndex].material),
                                    ray, hit, shadingBuffers[BufferType::dielectric].get());
                            break;
                        default:
                            //assert(0);
                            break;
                    }
                }
                else
                {
                    rayData[SKYBOX]->push_back(ray);
                }
            }
            // compute shading for each material
            shade(shadingBuffers, rayData, out);
            ++iterations;
        }
        while (iterations != maxDepth && rayData[out]->size() != 0);

        // apply skybox
        const auto box = Skybox();
        for (auto &ray : *rayData[SKYBOX])
        {
            ray.color *= box(ray);
        }
    }

    void writeRays(std::vector<Vec3>& image) const
    {
        rasterize(*rayData[out], image);
        rasterize(*rayData[ABSORBED], image);
        rasterize(*rayData[SKYBOX], image);
    }

private:
    int out = -1;// index of the last destination buffer (double buffering)
    std::shared_ptr<const World> world;
    std::vector<HitRecord> hits;
    RayBuffers rayData;
    BufferMap shadingBuffers;
};
