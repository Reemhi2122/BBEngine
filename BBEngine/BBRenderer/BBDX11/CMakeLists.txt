# CMakeList.txt : CMake project for BBRenderer, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

add_library (BBRenderer_DX11 STATIC 
"Source/Graphics/Graphics.cpp"
"Source/Bindable/DX11Topology.cpp" 
"Source/Bindable/DX11IndexBuffer.cpp" 
"Source/Bindable/DX11VertexBuffer.cpp"
"Source/Bindable/PixelShader.cpp"
"Source/Bindable/VertexShader.cpp"
"Source/Bindable/DX11InputLayout.cpp"

"Source/Bindable/DX11Texture.cpp"
"Source/Bindable/DX11Sampler.cpp"
"Source/Bindable/CubeMap.cpp"
)

target_compile_definitions(BBRenderer_DX11 PUBLIC BBDX11)

target_include_directories(BBRenderer_DX11 PUBLIC
"Include/"
"../Shader/"
)

target_link_libraries(BBRenderer_DX11
ImGUI
BBMath
stb_image
Common
RendererCommon
)