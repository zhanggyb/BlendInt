# CMake file for BlendInt project
#

file(GLOB BlendIntUI_SRC "*.cpp")

add_library (BlendIntUI ${BlendIntUI_SRC})

target_link_libraries(BlendIntUI BlendIntCore BlendIntOpenGL BlendIntService ${LIBS})

