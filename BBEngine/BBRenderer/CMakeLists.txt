# CMakeList.txt : CMake project for BBRenderer, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

add_library (Renderer STATIC 
"BBRenderer.cpp"
 "Window/BBWindow.cpp" 
 "BBException/BBException.cpp"
 "Input/Keyboard.cpp" 
 "Input/Mouse.cpp" 
 "Graphics/Graphics.cpp"
 "DXERR/dxerr.cpp"
 "Shader/VertexShader.hlsl" 
 "Bindable/Topology.cpp" "Bindable/IndexBuffer.cpp")

target_include_directories(Renderer PUBLIC
"Include")