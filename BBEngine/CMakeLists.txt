# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.

cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project ("BBEngine")

# Include sub-projects.
add_subdirectory ("BBRenderer")
add_subdirectory ("BBEngine")
add_subdirectory ("BBMath")
add_subdirectory ("BBGame")
add_subdirectory ("UnitTest")
add_subdirectory ("Lib/imgui")
add_subdirectory ("Lib/stb_image")
