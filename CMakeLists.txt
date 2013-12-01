# CMake file for BlendInt project
#

cmake_minimum_required(VERSION 2.8.11)

project (BlendInt)

set(CMAKE_BUILD_TYPE_INIT "Debug")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/"
    ${CMAKE_MODULE_PATH})

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_definitions(-D__UNIX__)
    add_definitions(-D__LINUX__)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_definitions(-D__WIN__)
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(APPLE TRUE)
    add_definitions(-D__UNIX__)
    add_definitions(-D__APPLE__)
endif()

find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
    include_directories(${OPENGL_INCLUDE_DIR})
    set(LIBS ${LIBS} ${OPENGL_LIBRARIES})
endif()

#find_package(glfw REQUIRED)
if(APPLE)
    set(LIBS ${LIBS} glfw3)
else()
    set(LIBS ${LIBS} glfw)
endif()

#find_package(fontconfig REQUIRED)
set(LIBS ${LIBS} fontconfig)

find_package(GLEW REQUIRED)
if(GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    set(LIBS ${LIBS} ${GLEW_LIBRARIES})
endif()

if(APPLE)
    include_directories(/usr/local/include/freetype2)
    set(LIBS ${LIBS} freetype)
else()
    find_package(Freetype REQUIRED)
    if(FREETYPE_FOUND)
        include_directories(${FREETYPE_INCLUDE_DIRS})
        set(LIBS ${LIBS} ${FREETYPE_LIBRARIES})
    endif()
endif()

find_package(Boost REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    set(LIBS ${LIBS} ${Boost_LIBRARIES} boost_system boost_filesystem)
endif()

#add OpenImageIO link
if(APPLE)
    # todo: install openimageio
else()
    set(LIBS ${LIBS} OpenImageIO)
endif()

# add rt library
if(APPLE)
    # todo: install openimageio
else()
set(LIBS ${LIBS} rt)
endif()

include_directories(${BlendInt_SOURCE_DIR}/include)
include_directories(${BlendInt_SOURCE_DIR}/include/BlendInt)
include_directories(${BlendInt_SOURCE_DIR}/extern/CppEvents/src)

if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR
        CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    add_definitions(-DDEBUG)
endif()

if(MSVC)
    # Force to always compile with W4
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    # Update if necessary
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
endif()

#set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
#set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

add_subdirectory(extern)
add_subdirectory(src)

if(APPLE)
#add_subdirectory(test)
else()
    add_subdirectory(test)
endif()

add_subdirectory(demo)
