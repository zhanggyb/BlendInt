# CMake file for BlendInt project
#

include_directories(${BlendInt_SOURCE_DIR}/test)
include_directories(${BlendInt_SOURCE_DIR}/test/gtest)

#set(LIBS ${LIBS} glfw)
if(APPLE)
    set(LIBS ${LIBS} glfw3)
else()
    set(LIBS ${LIBS} glfw)
endif()

add_subdirectory(gtest)
add_subdirectory(Common)
add_subdirectory(unit)

