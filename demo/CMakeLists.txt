# CMake file for BIL project
#

file(GLOB sourcefiles "*/*.cpp")
list(APPEND demo_SRC ${sourcefiles})
file(GLOB sourcefiles "*.cpp")
list(APPEND demo_SRC ${sourcefiles})
file(GLOB headerfiles "*.hpp")
list(APPEND demo_SRC ${headfiles})
file(GLOB headerfiles "*/*.hpp")
list(APPEND demo_SRC ${headfiles})

add_executable(demo ${demo_SRC})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
target_link_libraries(demo BIL ${LIBS})
