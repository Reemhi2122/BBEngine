cmake_minimum_required(VERSION 3.14)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/f8d7d77c06936315286eb55f8de22cd23c188571.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
	BBETesting
	"AllocatorTest.cpp"
	"MathTest.cpp" 
	"ContainerTest.cpp" 
	"ThreadPoolTest.cpp" 
	"DataLoadingTest.cpp")

target_link_libraries(
  BBETesting
  GTest::gtest_main
  BBEngine
)

add_custom_target(game_copy_assets_unit_test ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/BBGame/Assets"
    "${CMAKE_CURRENT_BINARY_DIR}/Assets"
    COMMENT "Copying DX11 asset folder to output d1 folder")

    
add_dependencies(BBETesting game_copy_assets_unit_test)

include(GoogleTest)
gtest_discover_tests(BBETesting)