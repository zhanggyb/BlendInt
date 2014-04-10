# CMake file for BlendInt project
#

file(GLOB test_SRC "*.cpp")

add_executable(testButtonGroup ${test_SRC})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
target_link_libraries(testButtonGroup BlendInt Common gtest ${LIBS})
