# CMake file for BlendInt project
#

file(GLOB BlendIntFont_SRC "*.cpp")

add_library (BlendIntFont ${BlendIntFont_SRC})

target_link_libraries(BlendIntFont ${LIBS})

