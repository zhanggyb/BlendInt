# CMake file for BlendInt project
#

include_directories(${BlendInt_SOURCE_DIR}/test)

add_subdirectory(Common)
#add_subdirectory(unit)
add_subdirectory(gtest)

