# CMakeList.txt : CMake project for BBRenderer, include source and define
# project specific logic here.

cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (BBEngine 
"main.cpp" 
"BBEngine.h"
"BBEngine.cpp" 
"Timer/BBTimer.h"
"Timer/BBTimer.cpp")

# TODO: Add tests and install targets if needed.
target_link_libraries(BBEngine 
Renderer)