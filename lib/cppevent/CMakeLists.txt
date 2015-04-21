# CMake file for libCppEvent project
#

file(GLOB Header_Files "${PROJECT_SOURCE_DIR}/include/blendint/cppevent/*.hpp")
file(GLOB Source_Files "*.cpp")

add_library (cppevent ${Header_Files} ${Source_Files})

install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/blendint/cppevent" DESTINATION
    "include/blendint")
