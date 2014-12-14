# CMake file for BlendInt project
#

file(GLOB BlendIntCore_SRC "*.cpp")

add_library (BlendIntCore ${BlendIntCore_SRC})

target_link_libraries(BlendIntCore ${LIBS})

