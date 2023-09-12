# CMakeList.txt : CMake project for BBRenderer, include source and define
# project specific logic here.

cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (BBEngine 
"main.cpp" 
"BBEngine.h"
"BBEngine.cpp" 
"Timer/BBTimer.h"
"Timer/BBTimer.cpp"
"Logger/Logger.h"
"Logger/logger.cpp" 
"Utility/HelperFunctions.h" 
"Thread/Thread.cpp" 
"Thread/Thread.h" "Allocators/ArenaAlloc.h" "Allocators/ArenaAlloc.cpp" "Allocators/Allocator.cpp" "Allocators/Allocator.h")

# TODO: Add tests and install targets if needed.
target_link_libraries(BBEngine 
Renderer
BBMath
)

#copy shaders when a change happened.
add_custom_target(game_copy_shaders ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/BBRenderer/Shader"
    ${CMAKE_CURRENT_BINARY_DIR}/assets
    COMMENT "Copying DX12 asset folder to output d1 folder")

    
add_dependencies(BBEngine game_copy_shaders)