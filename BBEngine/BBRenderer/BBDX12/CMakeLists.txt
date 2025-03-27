# CMake file for Building Block Engine DX12 Renderer
cmake_minimum_required (VERSION 3.8)

add_library (BBRenderer_DX12 STATIC 
	"Source/test.cpp"
)

#set_target_properties(Renderer PROPERTIES PREFIX "BBRender")

target_include_directories(BBRenderer_DX12 PUBLIC
"Include/"
"Shader/"
)

target_link_libraries(BBRenderer_DX12 
ImGUI
BBMath
stb_image
Common
)