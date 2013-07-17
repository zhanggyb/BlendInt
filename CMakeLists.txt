# CMake file for BIL project
#

cmake_minimum_required(VERSION 2.8)

project (BIL)

set(CMAKE_BUILD_TYPE_INIT "Debug")

find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
    include_directories(${OPENGL_INCLUDE_DIR})
    set(LIBS ${LIBS} ${OPENGL_LIBRARIES})
endif()

#find_package(glfw REQUIRED)
set(LIBS ${LIBS} glfw)

#find_package(fontconfig REQUIRED)
set(LIBS ${LIBS} fontconfig)

find_package(GLEW REQUIRED)
if(GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    set(LIBS ${LIBS} ${GLEW_LIBRARIES})
endif()

find_package(Freetype REQUIRED)
if(FREETYPE_FOUND)
    include_directories(${FREETYPE_INCLUDE_DIRS})
    set(LIBS ${LIBS} ${FREETYPE_LIBRARIES})
endif()

find_package(Boost REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    set(LIBS ${LIBS} ${Boost_LIBRARIES} boost_system boost_filesystem)
endif()

include_directories(${BIL_SOURCE_DIR}/include)
include_directories(${BIL_SOURCE_DIR}/include/BIL)
include_directories(${BIL_SOURCE_DIR}/extern/CppEvents/src)

add_definitions(-DDEBUG)
add_definitions(-D__LINUX__)

#set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
#set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

add_subdirectory(extern)
add_subdirectory(src)
add_subdirectory(test)

