# CMake file for BlendInt project
#

file(GLOB BlendIntCore_HEADERS "${PROJECT_SOURCE_DIR}/intern/blendint/include/blendint/core/*.hpp")
file(GLOB BlendIntCore_SRC "*.cpp")

add_library (BlendIntCore ${BlendIntCore_HEADERS} ${BlendIntCore_SRC})

target_link_libraries(BlendIntCore ${LIBS})

