# CMake file for BlendInt project
#

file(GLOB opengl_SRC "*.cpp")

add_library (opengl ${opengl_SRC})

target_link_libraries(opengl ${LIBS})

