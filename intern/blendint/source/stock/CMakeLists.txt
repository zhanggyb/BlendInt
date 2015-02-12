# CMake file for BlendInt project
#

file(GLOB BlendIntStock_HEADERS "${PROJECT_SOURCE_DIR}/intern/blendint/include/blendint/stock/*.hpp")
file(GLOB BlendIntStock_SRC "*.cpp")

add_library (BlendIntStock ${BlendIntStock_HEADERS} ${BlendIntStock_SRC})

target_link_libraries(BlendIntStock BlendIntCore BlendIntOpenGL BlendIntGui ${LIBS})

