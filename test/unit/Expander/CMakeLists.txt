# CMake file for BlendInt project
#

file(GLOB test_SRC "*.cpp")

add_executable(testExpander ${test_SRC})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
target_link_libraries(testExpander BlendInt Common gtest ${LIBS})
