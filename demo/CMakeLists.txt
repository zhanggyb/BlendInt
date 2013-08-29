# CMake file for BIL project
#

file(GLOB demo_SRC "*/*.cpp")
list(APPEND demo_SRC "main.cpp")

add_executable(demo ${demo_SRC})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
target_link_libraries(demo BIL ${LIBS})
