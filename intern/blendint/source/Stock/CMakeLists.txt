# CMake file for BlendInt project
#

file(GLOB BlendIntStock_SRC "*.cpp")

add_library (BlendIntStock ${BlendIntStock_SRC})

target_link_libraries(BlendIntStock BlendIntCore BlendIntOpenGL BlendIntGui ${LIBS})

