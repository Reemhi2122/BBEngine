# CMakeList.txt : CMake project for BBRenderer, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

#add_executable (BBRenderer WIN32 "BBRenderer.cpp" "BBRenderer.h")

add_library (Renderer STATIC 
"BBRenderer.h"
"BBRenderer.cpp"
)
