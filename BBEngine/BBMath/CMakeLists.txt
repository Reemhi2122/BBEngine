cmake_minimum_required (VERSION 3.8)

add_library (BBMath STATIC 
"source/Vector3.cpp"
)

target_include_directories(BBMath PUBLIC
"Include")
