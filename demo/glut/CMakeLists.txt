# CMake file for BIL project
#

file(GLOB sourcefiles "*/*.cpp")
list(APPEND glut_demo_SRC ${sourcefiles})
file(GLOB sourcefiles "*.cpp")
list(APPEND glut_demo_SRC ${sourcefiles})
file(GLOB headerfiles "*.hpp")
list(APPEND glut_demo_SRC ${headfiles})
file(GLOB headerfiles "*/*.hpp")
list(APPEND glut_demo_SRC ${headfiles})

set(LIBS ${LIBS} glut)

add_executable(glut_demo ${glut_demo_SRC})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
target_link_libraries(glut_demo BIL ${LIBS})

