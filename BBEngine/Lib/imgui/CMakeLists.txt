cmake_minimum_required (VERSION 3.8)

add_library(ImGUI STATIC
	"imgui.cpp"
	"imgui_draw.cpp"
	"imgui_demo.cpp"
	"imgui_widgets.cpp"
	"imgui_tables.cpp"
	"backends/imgui_impl_win32.cpp"
	"backends/imgui_impl_dx11.cpp"
	"backends/imgui_impl_dx12.cpp"
)

target_include_directories(ImGUI PUBLIC 
${CMAKE_CURRENT_SOURCE_DIR}
"backends")
