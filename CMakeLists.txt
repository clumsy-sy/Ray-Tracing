cmake_minimum_required(VERSION 3.10)
project(RayTracing)
set(CMAKE_CXX_STANDARD 20)

aux_source_directory(src/camera CAMERA)
aux_source_directory(src/external EXTERNAL)
aux_source_directory(src/geometry GEOMETRY)
aux_source_directory(src/renderer RENDERER)
aux_source_directory(src/scene SCENE)
aux_source_directory(src/texture TEXTURE)
aux_source_directory(src/vector VECTOR)

add_executable(RayTracing src/main.cpp src/global.hpp ${CAMERA} ${EXTERNAL} ${GEOMETRY} ${RENDERER} ${SCENE} ${TEXTURE} ${VECTOR})

# target_compile_options(RayTracing PUBLIC -Wall -Wextra -pedantic -Wshadow -Wreturn-type -fsanitize=undefined)
target_compile_options(RayTracing PUBLIC -Wall -Wextra -pedantic -Wshadow -Wreturn-type -march=native)
target_compile_features(RayTracing PUBLIC cxx_std_20)
target_link_libraries(RayTracing PUBLIC)
