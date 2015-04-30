# CMake file for BlendInt project
#

cmake_minimum_required(VERSION 2.8.11)

project (BlendInt)

set(CMAKE_BUILD_TYPE_INIT "Debug")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(BLENDINT_VERSION_FULL "0.1")
set(BLENDINT_LIB_NAME "blendint")

# Check the system and define BLENDINT_SYSTEM_NAME to:
#   0 - Linux
#   1 - OS X
#   2 - Windows (not supported currently)
# These values are also defined in include/blendint/config.hpp.in
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set(OS_LINUX TRUE)
  add_definitions(-DGL_GLEXT_PROTOTYPES)
  set(BLENDINT_SYSTEM_NAME 0)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(OS_APPLE TRUE)
  set(BLENDINT_SYSTEM_NAME 1)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  message(FATAL_ERROR "BlendInt does not support Windows yet!")
  set(OS_WINDOWS TRUE)
  set(BLENDINT_SYSTEM_NAME 2)
endif()

# start of options
option(BUILD_STATIC_LIBRARY "Build static library instead of shared" OFF)
option(ENABLE_OPENCV "Enable OpenCV Support" OFF)
option(WITH_GPERFTOOLS "Build with Google perftools option" OFF)
option(WITH_ALL_DEMOS "Build all demo programs" OFF)
option(WITH_GLFW3_DEMO "Build GLFW3 demo program" OFF)
option(WITH_GLUT_DEMO "Build GLUT demo program" OFF)
option(WITH_QT5_DEMO "Build Qt5 demo program" OFF)
option(WITH_UNIT_TEST "Build unit test code" OFF)
option(BUILD_DOCUMENTATION "Use Doxygen to create the HTML based API documentation" OFF)
# end of options

configure_file("${PROJECT_SOURCE_DIR}/include/blendint/config.hpp.in"
  "${PROJECT_BINARY_DIR}/include/blendint/config.hpp" @ONLY)

# Check if compiler support C++11:

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
  message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no
    C++11 support. Please use a different C++ compiler.")
endif()

# to find the config header:
include_directories("${PROJECT_BINARY_DIR}/include")

# for cppevent:
include_directories("${PROJECT_SOURCE_DIR}/include/blendint")

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

# add OpenImageIO link
find_package(OpenImageIO REQUIRED)
if(OPENIMAGEIO_FOUND)
  include_directories(${OPENIMAGEIO_INCLUDE_DIR})
  set(LIBS ${LIBS} ${OPENIMAGEIO_LIBRARY})
endif()

# search pkg-config
find_package(PkgConfig)
if(PKG_CONFIG_FOUND)
  configure_file("${PROJECT_SOURCE_DIR}/lib/pkgconfig/blendint.pc.in"
    "${PROJECT_BINARY_DIR}/lib/pkgconfig/blendint.pc" @ONLY)
endif()

# configure glfw
find_package(GLFW REQUIRED)
if(GLFW_FOUND)
  include_directories(${GLFW_INCLUDE_DIR})
  set(LIBS ${LIBS} ${GLFW_LIBRARIES})
endif()

if(OS_APPLE)
  add_definitions(-DGLFW_INCLUDE_GLCOREARB)
  add_definitions(-DGLFW_INCLUDE_GLEXT)
endif()

# glm
find_package(GLM REQUIRED)
if(GLM_FOUND)
include_directories(${GLM_INCLUDE_DIRS})
add_definitions(-DGLM_FORCE_RADIANS)
endif()

if(ENABLE_OPENCV)
  find_package(OpenCV REQUIRED)
  add_definitions(-D__USE_OPENCV__)
  set(LIBS ${LIBS} ${OpenCV_LIBS})
endif()

include_directories(${BlendInt_SOURCE_DIR}/include)


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

# add rapidxml header files
include_directories(${BlendInt_SOURCE_DIR}/extern/rapidxml)

if(OS_APPLE)
  # more settings for OS X OS X RPATH:
  # http://www.cmake.org/Wiki/CMake_RPATH_handling#Mac_OS_X_and_the_RPATH
  set(CMAKE_MACOSX_RPATH 1)
  set(LIBS ${LIBS} pthread)
endif()

add_subdirectory(lib)
add_subdirectory(editor)
add_subdirectory(data)

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

# ----------------------------------------------------------------------------
# Summary:
# ----------------------------------------------------------------------------

message(STATUS "")
message(STATUS "  config.hpp is in: ${PROJECT_BINARY_DIR}/include/blendint")
message(STATUS "  Install path: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "")

