#include <iostream>
#include <chrono>
#include "Camera.h"
#include "World.h"
#include "Util.h"
#include "Parser.h"
#include "Renderer.h"

int main(int argc, char* argv[])
{
    // settings with default values
    auto resolutionWidth = 400;
    auto resolutionHeight = 200;
    auto antialiasing = 1;
    auto maxBounces = 2;
    auto path = std::string{"../scene.txt"};

    parseCmdArguments(argc, argv, resolutionWidth, resolutionHeight, antialiasing, maxBounces, path);

    // print settings
    std::cout << "resolutionWidth: " << resolutionWidth << std::endl;
    std::cout << "resolutionHeight: " << resolutionHeight << std::endl;
    std::cout << "antialiasing: " << antialiasing << std::endl;
    std::cout << "maxBounces: " << maxBounces << std::endl;
    std::cout << "path: " << path << std::endl;

    auto world = std::make_shared<World>();
    parseWorld(path, *world);

    auto numPixels = resolutionWidth * resolutionHeight;
    auto numRays = numPixels * antialiasing;

    // image generated, stored as a 1D color array
    std::vector<Vec3> image(numPixels);

    // setup rays and pixels, double buffering
    std::vector<Ray> rays;
    rays.reserve(numRays);

    // setup camera
    const Vec3 lookFrom(0, 0, 4);
    const Vec3 lookAt(0, 0, -1);
    const auto distToFocus = (lookFrom - lookAt).length();
    const auto aperture = 0.f;
    Camera camera(lookFrom, lookAt, Vec3(0, 1, 0), 20, float(resolutionWidth) / float(resolutionHeight), aperture, distToFocus);

    // initialize rays based on camera
    camera.initializeRays(rays, resolutionWidth, resolutionHeight, antialiasing);

    Renderer renderer(rays, world);

    const auto tStart = std::chrono::high_resolution_clock::now();

    renderer.render(maxBounces);

    renderer.writeRays(image);

    // normalize pixels according to resolution
    for (auto& pixel : image)
    {
        pixel /= (float)antialiasing;
    }

    // write output image
    writeImagePPM(image, resolutionWidth, resolutionHeight, "out");

    const auto tEnd = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( tEnd - tStart ).count();
    std::cout << duration << " microsecs" << std::endl;

    return 0;
}