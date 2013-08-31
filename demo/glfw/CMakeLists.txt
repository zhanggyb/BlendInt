# CMake file for BIL project
#

file(GLOB sourcefiles "*/*.cpp")
list(APPEND glfw_demo_SRC ${sourcefiles})
file(GLOB sourcefiles "*.cpp")
list(APPEND glfw_demo_SRC ${sourcefiles})
file(GLOB headerfiles "*.hpp")
list(APPEND glfw_demo_SRC ${headfiles})
file(GLOB headerfiles "*/*.hpp")
list(APPEND glfw_demo_SRC ${headfiles})

set(LIBS ${LIBS} glfw)

add_executable(glfw_demo ${glfw_demo_SRC})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
target_link_libraries(glfw_demo BIL ${LIBS})

