# CMake file for Building Block Engine DX12 Renderer
cmake_minimum_required (VERSION 3.8)

add_library (BBRenderer_DX12 STATIC 
	"Source/test.cpp"
	"Source/Graphics.cpp"
	"Source/Bindable/DX12Texture.cpp"
	"Source/Bindable/DX12VertexBuffer.cpp" 
	"Source/Bindable/DX12IndexBuffer.cpp"
	"Source/DescriptorFreeList.cpp")

target_compile_definitions(BBRenderer_DX12 PUBLIC BBDX12)

target_include_directories(BBRenderer_DX12 PUBLIC
"Include/"
"Common/"
"../Shader/"
)

target_link_libraries(BBRenderer_DX12 
ImGUI
BBMath
stb_image
Common
RendererCommon
)