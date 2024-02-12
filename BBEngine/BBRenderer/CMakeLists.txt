# CMakeList.txt : CMake project for BBRenderer, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

add_library (Renderer STATIC 
"Source/BBRenderer.cpp"
"DXERR/dxerr.cpp"

"Source/BBException/BBException.cpp"
"Source/Graphics/Graphics.cpp"
"Source/Bindable/Topology.cpp" 
"Source/Bindable/IndexBuffer.cpp" 
"Source/Bindable/VertexBuffer.cpp"
"Source/Bindable/PixelShader.cpp"
"Source/Bindable/VertexShader.cpp"
"Source/Bindable/InputLayout.cpp"
"Source/Drawable/Drawable.cpp"
"Source/Bindable/TransformBuf.cpp"
"Source/Drawable/Box.cpp"
"Source/Drawable/Model.cpp"
"Source/Bindable/Texture.cpp"
"Source/Bindable/Sampler.cpp"

"Source/DirectionalLight.cpp"
"Source/SpotLight.cpp"

"Source/Camera.cpp")

set_target_properties(Renderer PROPERTIES PREFIX "BBRenderr")

target_include_directories(Renderer PUBLIC
"Include/"
)

target_link_libraries(Renderer
ImGUI
BBMath
stb_image
)