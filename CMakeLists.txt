# CMake file for BIL project
#

cmake_minimum_required(VERSION 2.6)

project (BIL)

#find_package(glfw REQUIRED)
#set(LIBS ${LIBS} ${glfw_LIBRAIES})

#find_package(glew REQUIRED)
#set(LIBS ${LIBS} ${glfw_LIBRAIES})

set(LIBS ${LIBS} GL glfw glew freetype)

include_directories(${BIL_SOURCE_DIR}/include)
include_directories(${BIL_SOURCE_DIR}/include/BIL)
include_directories(${BIL_SOURCE_DIR}/extern/CppEvents/src)
include_directories(/usr/include/freetype2)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

add_subdirectory(extern)
add_subdirectory(src)

