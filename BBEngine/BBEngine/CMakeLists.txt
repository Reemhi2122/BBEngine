# CMakeList.txt : CMake project for BBRenderer, include source and define
# project specific logic here.

cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_library (BBEngine 
"Source/BBEngine.cpp"
"Source/Timer/BBTimer.cpp"
"Source/Logger/logger.cpp" 
"Source/Thread/ThreadPool.cpp"
 
"Source/Allocators/StackAllocator.cpp" 
"Source/Allocators/ArenaAllocator.cpp" 
"Source/Allocators/PoolAllocator.cpp"
"Source/Allocators/FreeListAllocator.cpp"
"Source/Allocators/BuddyAllocator.cpp"
"Source/Allocators/Allocator.cpp" 

"Source/Window/BBWindow.cpp" 

"Source/Input/Keyboard.cpp" 
"Source/Input/Mouse.cpp" 

"Source/System/FileHandler/FileHandler_Windows.cpp"
"Source/System/BBFStream.cpp"

"Source/Utility/ImageProcessing.cpp"

"Source/FileLoaders/Sound/WAVLoader.cpp"
"Source/FileLoaders/Image/BMPLoader.cpp"
"Source/FileLoaders/Data/JsonParser.cpp"
"Source/FileLoaders/Data/INIParser.cpp"
"Source/FileLoaders/Models/GLTFParser.cpp" 

"Source/GameLib/BBObject.cpp"
#"Source/UserInterface.cpp" 
"Source/GameLib/BBComponent.cpp"

#components
#"Source/GameLib/Components/LightComponents/DirectionalLightComponent.cpp" 
#"Source/GameLib/Components/LightComponents/PointLightComponent.cpp" 
#"Source/GameLib/Components/LightComponents/SpotLightComponent.cpp"

"Source/Lights/DirectionalLight.cpp"
"Source/Lights/SpotLight.cpp"
"Source/Lights/PointLight.cpp"

#"Source/GameLib/Components/MeshComponent.cpp" 
"Source/GameLib/Components/Transform.cpp"
)

set (CMAKE_INCLUDE_DIRECTORIES_BEFORE ON)

# TODO: Add tests and install targets if needed.
target_link_libraries(BBEngine 
#BBRenderer_DX12
BBRenderer_DX11
BBMath
ImGUI
Common
stb_image
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
