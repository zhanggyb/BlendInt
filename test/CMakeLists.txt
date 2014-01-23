# CMake file for BlendInt project
#

include_directories(${BlendInt_SOURCE_DIR}/test)
include_directories(${BlendInt_SOURCE_DIR}/test/gtest/include)

add_subdirectory(gtest)
add_subdirectory(Common)
add_subdirectory(unit)

