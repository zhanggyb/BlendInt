# CMake file for libCppEvent project
#

file(GLOB Header_Files "${PROJECT_SOURCE_DIR}/intern/blendint/include/cppevent/*.hpp")
file(GLOB Source_Files "*.cpp")

add_library (cppevent ${Header_Files} ${Source_Files})

