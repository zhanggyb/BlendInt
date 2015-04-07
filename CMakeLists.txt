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
option(BUILD_DOCUMENTATION "Use Doxygen to create the HTML based API documentation" OFF)
# end of options

configure_file(
	"${PROJECT_SOURCE_DIR}/config.hpp.in"
	"${PROJECT_BINARY_DIR}/config.hpp"
)

include_directories("${PROJECT_BINARY_DIR}")	# so that we can find config header

if(OS_APPLE)
    set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} /usr/local/lib)
endif()

find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
    include_directories(${OPENGL_INCLUDE_DIR})
    set(LIBS ${LIBS} ${OPENGL_LIBRARIES})
endif()

find_package(Freetype REQUIRED)
if(FREETYPE_FOUND)
    include_directories(${FREETYPE_INCLUDE_DIRS})
    set(LIBS ${LIBS} ${FREETYPE_LIBRARIES})
endif()

find_package(FontConfig REQUIRED)
if(FONTCONFIG_FOUND)
    include_directories(${FONTCONFIG_INCLUDE_DIR})
    set(LIBS ${LIBS} ${FONTCONFIG_LIBRARY})
endif()

#find_package(GLEW REQUIRED)
#if(GLEW_FOUND)
#    include_directories(${GLEW_INCLUDE_DIRS})
#    set(LIBS ${LIBS} ${GLEW_LIBRARIES})
#endif()

find_package(Boost REQUIRED COMPONENTS filesystem system thread)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    set(LIBS ${LIBS} ${Boost_LIBRARIES})
    #message(STATUS "Use the Boost libraries: ${Boost_LIBRARIES}")
    #message(STATUS "Use the Boost libraries in: ${Boost_LIBRARY_DIRS}")
endif()

#add OpenImageIO link
find_package(OpenImageIO REQUIRED)
if(OPENIMAGEIO_FOUND)
    include_directories(${OPENIMAGEIO_INCLUDE_DIR})
    set(LIBS ${LIBS} ${OPENIMAGEIO_LIBRARY})
endif()

if(OS_APPLE)
    set(LIBS ${LIBS} pthread)
else()
    # add rt library
    set(LIBS ${LIBS} rt)
endif()

include_directories(${BlendInt_SOURCE_DIR}/intern/blendint/include)
include_directories(${BlendInt_SOURCE_DIR}/intern/blendint/include/cppevents)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no
    C++11 support. Please use a different C++ compiler.")
endif()

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

# configure glfw
if(WITH_BUILTIN_GLFW3)
    add_subdirectory(extern/glfw)
    include_directories(extern/glfw/include)

    if(OS_LINUX)
      set(LIBS ${LIBS} Xcursor)   # Xcursor missing in latest glfw3
    endif()
else()
    if(OS_APPLE)
        set(LIBS ${LIBS} glfw3)
    else()
        set(LIBS ${LIBS} glfw)
    endif()
endif()

if(OS_APPLE)
    add_definitions(-DGLFW_INCLUDE_GLCOREARB)
    add_definitions(-DGLFW_INCLUDE_GLEXT)
endif()

# add rapidxml header files
include_directories(${BlendInt_SOURCE_DIR}/extern/rapidxml)

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

if(OS_APPLE)    # more settings for OS X
    # OS X RPATH: http://www.cmake.org/Wiki/CMake_RPATH_handling#Mac_OS_X_and_the_RPATH
    set(CMAKE_MACOSX_RPATH 1)
endif()

add_subdirectory(intern/blendint/source)
add_subdirectory(editor)

if(BUILD_STATIC_LIBRARY)
    set(LIBS ${LIBS} pthread)
endif()

if(BUILD_DOCUMENTATION)
    add_subdirectory(doc)
endif()

if(WITH_UNIT_TEST)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-variadic-macros")
	if(EXISTS "${BlendInt_SOURCE_DIR}/test")
	add_subdirectory(test)
	endif()
endif()

if(WITH_ALL_DEMOS)
    set(WITH_GLFW3_DEMO TRUE)
    set(WITH_GLUT_DEMO TRUE)
    set(WITH_QT5_DEMO TRUE)
endif()

if(EXISTS "${BlendInt_SOURCE_DIR}/demo")
	add_subdirectory(demo)
endif()

install(DIRECTORY "${PROJECT_SOURCE_DIR}/release/datafiles" DESTINATION share/blendint)

