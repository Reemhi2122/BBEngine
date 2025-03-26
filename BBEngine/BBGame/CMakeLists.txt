cmake_minimum_required (VERSION 3.8)

add_executable (BBGame
"main.cpp"
)

target_link_libraries(BBGame 
BBEngine
)

#copy shaders when a change happened.
add_custom_target(game_copy_shaders ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/BBRenderer_DX11/Shader"
    "${CMAKE_CURRENT_BINARY_DIR}/Assets"
    COMMENT "Copying DX11 asset folder to output d1 folder")

add_custom_target(game_copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/BBGame/Assets"
    "${CMAKE_CURRENT_BINARY_DIR}/Assets"
    COMMENT "Copying DX11 asset folder to output d1 folder")

    
add_dependencies(BBGame game_copy_shaders)
add_dependencies(BBGame game_copy_assets)