# CMakeList.txt : CMake project for BBRenderer, include source and define
# project specific logic here.

cmake_minimum_required (VERSION 3.8)



# Add source to this project's executable.
add_library (BBEngine 
"BBEngine.cpp" 
"Timer/BBTimer.cpp"
"Logger/logger.cpp" 
 
"Allocators/StackAllocator.cpp" 
"Allocators/ArenaAllocator.cpp" 
"Allocators/PoolAllocator.cpp"
"Allocators/FreeListAllocator.cpp"
"Allocators/BuddyAllocator.cpp"
"Allocators/Allocator.cpp" 
"Thread/ThreadPool.cpp"
"System/FileHandler/FileHandler_Windows.cpp"
)


# TODO: Add tests and install targets if needed.
target_link_libraries(BBEngine 
Renderer
BBMath
)

target_include_directories(BBEngine PUBLIC
"Include/"
)

#copy shaders when a change happened.
#add_custom_target(game_copy_shaders ALL
#    COMMAND ${CMAKE_COMMAND} -E copy_directory
#    "${CMAKE_SOURCE_DIR}/BBRenderer/Shader"
#    ${CMAKE_CURRENT_BINARY_DIR}/assets
#    COMMENT "Copying DX11 asset folder to output d1 folder")

    
#add_dependencies(BBEngine game_copy_shaders)
