# CMakeList.txt : CMake project for BBRenderer, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

#add_executable (BBRenderer WIN32 "BBRenderer.cpp" "BBRenderer.h")

add_library (Renderer STATIC 
"BBRenderer.cpp"
 "Window/BBWindow.cpp" 
 "BBException/BBException.cpp"
 "Input/Keyboard.cpp" 
 "Input/Mouse.cpp" 
 "Graphics/Graphics.cpp"
 "DXERR/dxerr.cpp"
 "Shader/VertexShader.hlsl"
)

target_include_directories(Renderer PUBLIC
"Include")