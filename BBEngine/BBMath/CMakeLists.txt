cmake_minimum_required (VERSION 3.8)

add_library (BBMath STATIC 
"source/Vector3.cpp"
"source/vector4.cpp"
"source/Matrix4x4.cpp")

target_include_directories(BBMath PUBLIC
"Include")
