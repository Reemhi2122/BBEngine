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
"Source/Bindable/PixelShader.cpp"
"Source/Bindable/VertexShader.cpp"
"Source/Bindable/InputLayout.cpp"
"Source/Drawable/Drawable.cpp"
"Source/Bindable/TransformBuf.cpp"
"Source/Drawable/Box.cpp")


target_include_directories(Renderer PUBLIC
"${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(Renderer
ImGUI
)