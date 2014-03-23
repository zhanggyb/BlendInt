# CMake file for BlendInt project
#

file(GLOB test_SRC "*.cpp")

add_executable(testFreetype ${test_SRC})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
target_link_libraries(testFreetype BlendInt Common gtest ${LIBS})
