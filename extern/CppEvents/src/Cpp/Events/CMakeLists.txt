# CMake file for BIL project
#
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
add_library (CppEvents ConnectionList.cpp Threading.cpp)
