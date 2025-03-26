cmake_minimum_required (VERSION 3.8)

add_library (Common STATIC 
	"Source/temp.cpp"
)

target_include_directories(Common PUBLIC
"Include/"
)