# CMake file for BlendInt project
#

file(GLOB OpenGL_SRC "*.cpp")

add_library (OpenGL ${OpenGL_SRC})

target_link_libraries(OpenGL ${LIBS})

