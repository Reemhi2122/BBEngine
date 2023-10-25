# CMakeList.txt : CMake project for BBRenderer, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

add_library (Renderer STATIC 
"BBRenderer.cpp"
"DXERR/dxerr.cpp"

"Source/Window/BBWindow.cpp" 
"Source/BBException/BBException.cpp"
"Source/Input/Keyboard.cpp" 
"Source/Input/Mouse.cpp" 
"Source/Graphics/Graphics.cpp"
"Source/Bindable/Topology.cpp" 
"Source/Bindable/IndexBuffer.cpp" 
"Source/Bindable/VertexBuffer.cpp"
"Source/Bindable/PixelShader.cpp" "Include/Bindable/VertexShader.h" "Source/Bindable/VertexShader.cpp" "Include/Bindable/InputLayout.h" "Source/Bindable/InputLayout.cpp" "Include/Drawable/Drawable.h" "Source/Drawable/Drawable.cpp")

target_include_directories(Renderer PUBLIC
"Include")


target_link_libraries(Renderer
ImGUI
)