cmake_minimum_required(VERSION 3.10)
project(RayTracing)
set(CMAKE_CXX_STANDARD 20)

MESSAGE(STATUS "Project: Ray Tracing!")

aux_source_directory(src/camera CAMERA)
aux_source_directory(src/external EXTERNAL)
aux_source_directory(src/geometry GEOMETRY)
aux_source_directory(src/renderer RENDERER)
aux_source_directory(src/scene SCENE)
aux_source_directory(src/texture TEXTURE)
aux_source_directory(src/vector VECTOR)

add_executable(RayTracing src/main.cpp src/global.hpp ${SCENE} ${CAMERA} ${EXTERNAL} ${GEOMETRY} ${RENDERER}  ${TEXTURE} ${VECTOR})


target_compile_options(RayTracing PUBLIC -O3 -march=native -std=c++20 -Wall -Wextra)

