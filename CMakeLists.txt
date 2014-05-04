# CMake file for BlendInt project
#

cmake_minimum_required(VERSION 2.8.11)

project (BlendInt)

set(CMAKE_BUILD_TYPE_INIT "Debug")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(OS_LINUX TRUE)
    add_definitions(-D__UNIX__)
    add_definitions(-D__LINUX__)
    add_definitions(-DGL_GLEXT_PROTOTYPES)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(OS_WINDOWS TRUE)
    add_definitions(-D__WIN__)
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(OS_APPLE TRUE)
    add_definitions(-D__UNIX__)
    add_definitions(-D__APPLE__)
endif()

# start of options
option(BUILD_STATIC_LIBRARY "Build static library instead of shared" OFF)
option(WITH_BUILTIN_GLFW3 "Build with built-in GLFW3 source code" ON)
option(WITH_BUILTIN_GLM "Build with built-in GLM headers" ON)
option(ENABLE_OPENCV "Enable OpenCV Support" OFF)
option(WITH_GPERFTOOLS "Build with Google perftools option" OFF)
option(WITH_ALL_DEMOS "Build all demo programs" OFF)
option(WITH_GLFW3_DEMO "Build GLFW3 demo program" OFF)
option(WITH_GLUT_DEMO "Build GLUT demo program" OFF)
option(WITH_QT5_DEMO "Build Qt5 demo program" OFF)
option(WITH_UNIT_TEST "Build unit test code" OFF)
# end of options

find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
    include_directories(${OPENGL_INCLUDE_DIR})
    set(LIBS ${LIBS} ${OPENGL_LIBRARIES})
    if(OS_APPLE)
        FIND_PATH(GL_H_INCLUDE_DIR gl.h)
        include_directories(${GL_H_INCLUDE_DIR})
    endif()
endif()

if(OS_APPLE)

    include_directories(/usr/local/include/freetype2)
    set(LIBS ${LIBS} freetype)

elseif(OS_LINUX)
    find_package(Freetype REQUIRED)
    if(FREETYPE_FOUND)
        include_directories(/usr/include/freetype2)
        include_directories(${FREETYPE_INCLUDE_DIRS})
        set(LIBS ${LIBS} ${FREETYPE_LIBRARIES})
    endif()
endif()

find_package(FontConfig REQUIRED)
if(FONTCONFIG_FOUND)
    add_definitions(-DUSE_FONTCONFIG)
    include_directories(${FONTCONFIG_INCLUDE_DIR})
    set(LIBS ${LIBS} ${FONTCONFIG_LIBRARY})
endif()

#find_package(GLEW REQUIRED)
#if(GLEW_FOUND)
#    include_directories(${GLEW_INCLUDE_DIRS})
#    set(LIBS ${LIBS} ${GLEW_LIBRARIES})
#endif()
add_definitions(-D__OPENGL_CORE_330__)

find_package(Boost REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    set(LIBS ${LIBS} ${Boost_LIBRARIES} boost_system boost_filesystem)
endif()

#add OpenImageIO link
if(OS_APPLE)
    set(LIBS ${LIBS} OpenImageIO)
else()
    set(LIBS ${LIBS} OpenImageIO)
endif()

if(OS_APPLE)
    set(LIBS ${LIBS} pthread)
else()
    # add rt library
    set(LIBS ${LIBS} rt)
endif()

include_directories(${BlendInt_SOURCE_DIR}/include)
include_directories(${BlendInt_SOURCE_DIR}/include/BlendInt)
include_directories(${BlendInt_SOURCE_DIR}/extern/CppEvents/src)
include_directories(${BlendInt_SOURCE_DIR}/extern/rapidxml)

if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR
        CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    add_definitions(-DDEBUG)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -p -pg")
    endif()
    if(WITH_GPERFTOOLS)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
        set(LIBS ${LIBS} profiler)
    endif()
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
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

# build glfw
if(WITH_BUILTIN_GLFW3)
    add_subdirectory(extern/glfw)
    include_directories(extern/glfw/include)

    if(OS_LINUX)
        set(LIBS ${LIBS} Xcursor)   # Xcursor missing in latest glfw3
    endif()
endif()

# include GLM
if(WITH_BUILTIN_GLM)
	# glm header in extern/glm
	include_directories(extern)
else()
    if(OS_APPLE)
	#include_directories(/usr/local/include/glm)
	else()
		find_package(GLM REQUIRED)
    		if(GLM_FOUND)
        		include_directories(${GLM_INCLUDE_DIRS})
    		endif()
	endif()
endif()
add_definitions(-DGLM_FORCE_RADIANS)

if(ENABLE_OPENCV)
    find_package(OpenCV REQUIRED)
    add_definitions(-D__USE_OPENCV__)
    set(LIBS ${LIBS} ${OpenCV_LIBS})
endif()

add_subdirectory(extern/CppEvents/src)
add_subdirectory(src)

if(BUILD_STATIC_LIBRARY)
    set(LIBS ${LIBS} pthread)
endif()

if(WITH_UNIT_TEST)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-variadic-macros")
    add_subdirectory(test)
endif()

if(WITH_ALL_DEMOS)
    set(WITH_GLFW3_DEMO TRUE)
    set(WITH_GLUT_DEMO TRUE)
    set(WITH_QT5_DEMO TRUE)
endif()

add_subdirectory(demo)

