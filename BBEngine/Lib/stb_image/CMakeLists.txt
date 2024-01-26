cmake_minimum_required (VERSION 3.8)

add_library(stb_image INTERFACE)

target_include_directories(stb_image INTERFACE .)

set_target_properties(stb_image PROPERTIES LINKER_LANGUAGE CXX)