cmake_minimum_required(VERSION 3.10)
project(OpenGL_Project)

set(CMAKE_CXX_STANDARD 17)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Add Include Directories
include_directories(include)

# Add Executable
add_executable(testing src/main.cpp src/glad.c)

# Link Libraries
target_link_libraries(testing OpenGL::GL glfw)
