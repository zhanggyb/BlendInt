# CMake file for BlendInt project
#

file(GLOB BlendIntGui_HEADERS "${PROJECT_SOURCE_DIR}/intern/blendint/include/blendint/gui/*.hpp")
file(GLOB BlendIntGui_SRC "*.cpp")

add_library (BlendIntGui ${BlendIntGui_HEADERS} ${BlendIntGui_SRC})

target_link_libraries(BlendIntGui BlendIntCore BlendIntOpenGL BlendIntStock BlendIntFont ${LIBS})

