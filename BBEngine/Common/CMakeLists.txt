cmake_minimum_required (VERSION 3.8)

add_library (Common STATIC 
	"Source/temp.cpp"
	"Source/BBException/BBException.cpp"
	"Source/DXERR/dxerr.cpp"
)

target_include_directories(Common PUBLIC
"Include/"
)