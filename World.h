//
// Created by etienne cella on 2018-05-20.
//

#pragma once
#include <vector>
#include <memory>
#include "Structs.h"
#include "Buffer.h"
#include "Sphere.h"

struct Mesh
{
    Mesh(BufferType primitiveType_,
         BufferType materialType_,
         Buffer&& geometry_,
         Buffer&& material_) :
            primitiveType(primitiveType_),
            materialType(materialType_),
            geometry(std::move(geometry_)),
            material(std::move(material_)) {}

    BufferType primitiveType;
    BufferType materialType;
    Buffer geometry;
    Buffer material;
};

// TODO expose iterators as we'll traverse the world using trees
class World
{
public:
    void traverse(const std::vector<Ray>& rays, std::vector<HitRecord>& hits) const
    {
        // TODO scene traversal should use an acceleration structure
        for (size_t i = 0; i != rays.size(); ++i)
        {
            const auto &ray = rays[i];
            auto &hitrec = hits[i];
            for (size_t j = 0; j != meshes.size(); ++j)
            {
                hit(*(meshes[j]), ray, hitrec, j);
            }
        }
    }

    // meshes are read only and not meant to be iterated on
    const Mesh& operator[](int index) const { return (*meshes[index]); }

    void append(std::unique_ptr<Mesh> mesh) { meshes.push_back(std::move(mesh)); }

private:

    bool hit(const Mesh& mesh, const Ray& ray, HitRecord& hit, int meshIndex) const
    {
        switch (mesh.primitiveType)
        {
            case BufferType::sphere: return SphereCollider()(mesh.geometry, ray, hit, meshIndex);
            default: return false;
        }
    }

    std::vector<std::unique_ptr<Mesh>> meshes;
};