# CMake file for BlendInt project
#

file(GLOB BlendIntService_SRC "*.cpp")

add_library (BlendIntService ${BlendIntService_SRC})

target_link_libraries(BlendIntService BlendIntUI ${LIBS})

