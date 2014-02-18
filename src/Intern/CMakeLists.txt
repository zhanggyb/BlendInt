# CMake file for BlendInt project
#

file(GLOB BlendIntIntern_SRC "*.cpp")

add_library (BlendIntIntern ${BlendIntIntern_SRC})

target_link_libraries(BlendIntIntern BlendIntCore BlendIntOpenGL ${LIBS})

