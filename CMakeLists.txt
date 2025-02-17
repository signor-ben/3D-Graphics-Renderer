cmake_minimum_required(VERSION 3.10)
project(OpenGL_Project)

set(CMAKE_CXX_STANDARD 17)

# Find OpenGL
find_package(OpenGL REQUIRED)

# If using vcpkg, set the toolchain file for automatic dependency management
set(CMAKE_TOOLCHAIN_FILE "vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

# Add Include Directories
include_directories(include)

# Link GLFW library directory
link_directories(vcpkg/installed/x64-windows/lib)

# Add Executable
add_executable(testing src/main.cpp src/glad.c src/stb_image.cpp)

# Link Libraries
target_link_libraries(testing PRIVATE OpenGL::GL glfw3dll)
