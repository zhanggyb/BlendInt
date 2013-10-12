# CMake file for BlendInt project
#

file(GLOB libBlendInt_SRC "*.cpp")

add_library (BlendInt SHARED ${libBlendInt_SRC})

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set_target_properties(BlendInt PROPERTIES VERSION 1 SOVERSION 1)
target_link_libraries(BlendInt CppEvents ${LIBS})
