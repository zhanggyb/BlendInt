# CMake file for BlendInt project
#

file(GLOB BlendIntOpenGL_SRC "*.cpp")

add_library (BlendIntOpenGL ${BlendIntOpenGL_SRC})

target_link_libraries(BlendIntOpenGL ${LIBS})

