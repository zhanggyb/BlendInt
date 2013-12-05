# CMake file for BlendInt project
#

file(GLOB test_SRC "*.cpp")

add_executable(testLayout ${test_SRC})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
target_link_libraries(testLayout BlendInt Common gtest ${LIBS})

