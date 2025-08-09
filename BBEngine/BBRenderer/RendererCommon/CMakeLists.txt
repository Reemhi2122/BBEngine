# CMake file for Building Block Engine DX12 Renderer
cmake_minimum_required (VERSION 3.8)

add_library (RendererCommon STATIC 
	"Camera.cpp"
 "Include/Bindable/ICubeMap.h")

target_include_directories(RendererCommon PUBLIC 
    "Include/"
)

target_link_libraries(RendererCommon 
BBMath
)