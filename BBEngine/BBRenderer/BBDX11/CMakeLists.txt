﻿# CMakeList.txt : CMake project for BBRenderer, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

add_library (BBRenderer_DX11 STATIC 
"Source/Graphics/Graphics.cpp"
"Source/Bindable/Topology.cpp" 
"Source/Bindable/IndexBuffer.cpp" 
"Source/Bindable/VertexBuffer.cpp"
"Source/Bindable/PixelShader.cpp"
"Source/Bindable/VertexShader.cpp"
"Source/Bindable/InputLayout.cpp"
"Source/Drawable/DrawToTexture.cpp"
"Source/Drawable/Box.cpp"
"Source/Drawable/Model.cpp"
"Source/Bindable/Texture.cpp"
"Source/Bindable/Sampler.cpp"
"Source/Bindable/CubeMap.cpp"

"Source/Drawable/SkyBox.cpp"
"Source/Drawable/Quad.cpp"
)

add_compile_definitions(BBX11)

target_include_directories(BBRenderer_DX11 PUBLIC
"Include/"
"../Shader/"
)

target_link_libraries(BBRenderer_DX11
ImGUI
BBMath
stb_image
Common
#RendererCommon
)