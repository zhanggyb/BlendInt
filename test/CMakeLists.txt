# CMake file for BlendInt project
#

include_directories(${BlendInt_SOURCE_DIR}/test)

#set(LIBS ${LIBS} glfw)
if(APPLE)
    set(LIBS ${LIBS} glfw3)
else()
    set(LIBS ${LIBS} glfw)
endif()

add_subdirectory(Common)
#add_subdirectory(unit)
add_subdirectory(gtest)

