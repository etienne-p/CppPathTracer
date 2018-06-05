cmake_minimum_required(VERSION 3.3)
project(InitiationRayTracer)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Vec3.h Vec3.cpp Lambertian.h Metal.h Camera.h Structs.h Sphere.h Skybox.h World.h Dielectric.h Util.h Buffer.h Parser.h Renderer.h)
add_executable(InitiationRayTracer ${SOURCE_FILES})