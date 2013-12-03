# CMake file for BlendInt project
#

list(APPEND LIBS gtest)

add_subdirectory(ClockWidget)
add_subdirectory(Layout)
add_subdirectory(StackedWidget)
add_subdirectory(TabFrame)
add_subdirectory(Timer)
add_subdirectory(Viewport3D)

#set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

