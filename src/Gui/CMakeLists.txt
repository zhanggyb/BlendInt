# CMake file for BlendInt project
#

file(GLOB BlendIntGui_SRC "*.cpp")

add_library (BlendIntGui ${BlendIntGui_SRC})

target_link_libraries(BlendIntGui BlendIntCore BlendIntOpenGL BlendIntStock ${LIBS})

