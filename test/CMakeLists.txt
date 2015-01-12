# CMake file for BlendInt project
#

if(OS_APPLE)
	add_definitions(-DGLFW_INCLUDE_GLCOREARB)
	add_definitions(-DGLFW_INCLUDE_GLEXT)
endif()

include_directories(${BlendInt_SOURCE_DIR}/test)
include_directories(${BlendInt_SOURCE_DIR}/test/gtest/include)

add_subdirectory(gtest)
add_subdirectory(Common)
add_subdirectory(unit)

