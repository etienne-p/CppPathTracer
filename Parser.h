//
// Created by etienne cella on 2018-05-29.
//

#pragma once
#include <sstream>
#include <fstream>
#include "Sphere.h"
#include "Lambertian.h"
#include "Metal.h"
#include "Dielectric.h"
#include "World.h"

template <typename T>
T fromStream(std::istringstream& stream) = delete;

template <>
Dielectric fromStream<Dielectric>(std::istringstream& stream)
{
    float arg;
    stream >> arg;
    return Dielectric(arg);
};

template <>
Lambertian fromStream<Lambertian>(std::istringstream& stream)
{
    float args[3];
    for (int i = 0; i != 3; ++i) stream >> args[i];
    return Lambertian(Vec3(args[0], args[1], args[2]));
};

template <>
Metal fromStream<Metal>(std::istringstream& stream)
{
    float args[4];
    for (int i = 0; i != 4; ++i) stream >> args[i];
    return Metal(Vec3(args[0], args[1], args[2]), args[3]);
};

template <>
Sphere fromStream<Sphere>(std::istringstream& stream)
{
    float args[4];
    for (int i = 0; i != 4; ++i) stream >> args[i];
    return Sphere(Vec3(args[0], args[1], args[2]), args[3]);
};

class BaseParser
{
public:
    virtual void parse(std::istringstream& stream) = 0;
    virtual Buffer asBuffer() = 0;
};

template <typename T>
class Parser : public BaseParser
{
public:
    void parse(std::istringstream& stream) override
    {
        data.push_back(fromStream<T>(stream));
    };
    Buffer asBuffer() override
    {
        Buffer buffer(data.size() * sizeof(T));
        StackView<T> stack(&buffer);
        for (const auto& s : data)
        {
            stack.push(s);
        }
        return buffer;
    };
private:
    std::vector<T> data;
};

std::unique_ptr<BaseParser> getParser(BufferType type)
{
    if (type == BufferType::lambertian)
    {
        return make_unique<Parser<Lambertian>>();
    }
    if (type == BufferType::metal)
    {
        return make_unique<Parser<Metal>>();
    }
    if (type == BufferType::sphere)
    {
        return make_unique<Parser<Sphere>>();
    }
    if (type == BufferType::dielectric)
    {
        return make_unique<Parser<Dielectric>>();
    }
    return nullptr;
}

void parseWorld(const std::string& path, World& world)
{
    std::ifstream infile(path);
    std::string line;

    while (std::getline(infile, line))
    {
        if (line.empty()) continue;

        std::istringstream stream(line);
        std::string word;
        stream >> word;

        if (word == "mesh")
        {
            std::string primitive, material;
            stream >> primitive;
            stream >> material;
            const auto primitiveType = stringToEnum(primitive);
            const auto materialType = stringToEnum(material);
            auto primitiveParser = getParser(primitiveType);
            auto materialParser = getParser(materialType);

            if (!(primitiveParser && materialParser))
            {
                // TODO fail, could not instantiate parsers
                assert(0);
                break;
            }

            while(getline(infile, line))
            {
                if (line.empty()) break;

                // TODO shadows previously defined stream
                std::istringstream stream(line);
                primitiveParser->parse(stream);
                materialParser->parse(stream);
            }

            // TODO treat empty meshes as errors
            world.append(
                    std::unique_ptr<Mesh>(new Mesh(
                    primitiveType, materialType,
                    primitiveParser->asBuffer(),
                    materialParser->asBuffer())));
        }
    }
}
