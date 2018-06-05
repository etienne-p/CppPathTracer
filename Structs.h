//
// Created by etienne cella on 2018-05-19.
//

#pragma  once

#include "Vec3.h"
#include "Buffer.h"

struct HitRecord
{
    HitRecord(const Vec3& point_, const Vec3& normal_, float t_, int meshIndex_, int primitiveIndex_) :
            point(point_), normal(normal_), t(t_), meshIndex(meshIndex_), primitiveIndex(primitiveIndex_) {}
    HitRecord()  : point(Vec3()), normal(Vec3()), t(-1), meshIndex(-1), primitiveIndex(-1) {}
    Vec3 point, normal;
    // will allow to retrieve shading parameters associated to a hit
    int meshIndex,primitiveIndex;
    float t;
};

struct Ray
{
    Ray(const Vec3& origin_, const Vec3& direction_, const Vec3& color_, int index_) :
            origin(origin_), direction(direction_), color(color_), index(index_){}
    Ray() : origin(Vec3()), direction(Vec3()), color(Vec3()), index(-1){}
    Vec3 pointAtParameter(float t) const {  return origin + direction * t; }
    Vec3 direction, origin, color;
    int index;
};

// Feed the shading stage, grouped as accessed together
template <typename T>
struct RayShadingData
{
    RayShadingData(const Ray& ray_, const HitRecord& hit_, const T& material_) :
            ray(ray_), hit(hit_), material(material_) {}
    Ray ray;
    HitRecord hit;
    T material;
};

// TODO add triangles,...
enum BufferType { none, metal, lambertian, dielectric, sphere };

BufferType stringToEnum(const std::string& str)
{
    if (str == "metal") return BufferType::metal;
    if (str == "lambertian") return BufferType::lambertian;
    if (str == "dielectric") return BufferType::dielectric;
    if (str == "sphere") return BufferType::sphere;
    return BufferType::none;
}


