# CMake file for BlendInt project
#

cmake_minimum_required(VERSION 2.8.11)

project (BlendInt)

set(CMAKE_BUILD_TYPE_INIT "Debug")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_definitions(-D__UNIX__)
    add_definitions(-D__LINUX__)
    add_definitions(-DGL_GLEXT_PROTOTYPES)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_definitions(-D__WIN__)
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(APPLE TRUE)
    add_definitions(-D__UNIX__)
    add_definitions(-D__APPLE__)
endif()

# start of options
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
endif()

if(APPLE)
    include_directories(/usr/local/include/freetype2)
    set(LIBS ${LIBS} freetype)
else()
    find_package(Freetype REQUIRED)
    if(FREETYPE_FOUND)
        include_directories(/usr/include/freetype2)
        include_directories(${FREETYPE_INCLUDE_DIRS})
        set(LIBS ${LIBS} ${FREETYPE_LIBRARIES})
    endif()
endif()

find_package(FontConfig REQUIRED)
if(FONTCONFIG_FOUND)
    include_directories(${FONTCONFIG_INCLUDE_DIR})
    set(LIBS ${LIBS} ${FONTCONFIG_LIBRARY})
endif()
#set(LIBS ${LIBS} fontconfig)

#find_package(GLEW REQUIRED)
#if(GLEW_FOUND)
#    include_directories(${GLEW_INCLUDE_DIRS})
#    set(LIBS ${LIBS} ${GLEW_LIBRARIES})
#endif()

find_package(Boost REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    set(LIBS ${LIBS} ${Boost_LIBRARIES} boost_system boost_filesystem)
endif()

if(APPLE)
    #include_directories(/usr/local/include/glm)
else()
find_package(GLM REQUIRED)
if(GLM_FOUND)
    include_directories(${GLM_INCLUDE_DIRS})
endif()
endif()

#add OpenImageIO link
if(APPLE)
    # use the libraries from Blender repository
    include_directories($ENV{HOME}/Opt/include)
    LINK_DIRECTORIES($ENV{HOME}/Opt/lib)
    set(LIBS ${LIBS} OpenImageIO)
else()
    set(LIBS ${LIBS} OpenImageIO)
endif()

if(APPLE)
    set(LIBS ${LIBS} pthread)
else()
    # add rt library
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
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

add_subdirectory(extern)
add_subdirectory(src)

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

