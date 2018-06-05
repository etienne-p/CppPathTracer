//
// Created by etienne cella on 2018-05-20.
//

#pragma once
#include <vector>
#include <fstream>
#include <iostream>
#include "Vec3.h"

// note that this is part of the standard library as of C++ 14
template <typename T, typename... Ts>
std::unique_ptr<T> make_unique(Ts&&... params)
{
    return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
};

template <typename T>
std::vector<T> make_vector_reserved(int reservedSize)
{
    auto v = std::vector<T>();
    v.reserve(reservedSize);
    return std::move(v);
}

void writeImagePPM(const std::vector<Vec3> &data, int width, int height, const std::string filenameNoExtension)
{
    std::ofstream outputFile;
    outputFile.open(filenameNoExtension + ".ppm");
    outputFile << "P3\n" << width << " " << height << "\n255\n";

    for (int y = height - 1; y != -1; --y)
    {
        for (int x = 0; x != width; ++x)
        {
            const auto color = data[y * width + x];
            outputFile <<
            int(255.99f * (sqrtf(color[0]))) << " " <<
            int(255.99f * (sqrtf(color[1]))) << " " <<
            int(255.99f * (sqrtf(color[2]))) << " ";
        }
        outputFile << "\n";
    }
    outputFile.close();
}

void parseCmdArguments(
        int argc, char* argv[],
        int& resolutionWidth,
        int& resolutionHeight,
        int& antialiasing,
        int& maxBounces,
        std::string& path)
{

    // we expect name - value pairs
    for (int i = 2; i < argc; i = i + 2)
    {
        if (argv[i-1][0] != '-')
        {
            std::cout << "argument name should start with [-]" << std::endl;
            continue;
        }
        switch(argv[i - 1][1])
        {
            case 'x': resolutionWidth  = atoi(argv[i]); break;
            case 'y': resolutionHeight = atoi(argv[i]); break;
            case 'a': antialiasing     = atoi(argv[i]); break;
            case 'd': maxBounces       = atoi(argv[i]); break;
            case 'f': path             = argv[i];       break;
            default: std::cout << "unexpected argument name: " << argv[i - 1] << std::endl; break;
        }
    }
}
