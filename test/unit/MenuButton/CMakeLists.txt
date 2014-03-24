# CMake file for BlendInt project
#

file(GLOB test_SRC "*.cpp")

add_executable(testMenuButton ${test_SRC})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
target_link_libraries(testMenuButton BlendInt Common gtest ${LIBS})
