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
	"Tests/AllocatorTest.cpp"
	"Tests/MathTest.cpp" 
	"Tests/ContainerTest.cpp" "Tests/ThreadPoolTest.cpp" "Tests/DataLoadingTest.cpp")

target_link_libraries(
  BBETesting
  GTest::gtest_main
  BBEngine
)

include(GoogleTest)
gtest_discover_tests(BBETesting)