# CMake file for BlendInt project
#

file(GLOB BlendIntFont_HEADERS "${PROJECT_SOURCE_DIR}/include/blendint/font/*.hpp")
file(GLOB BlendIntFont_SRC "*.cpp")

add_library (BlendIntFont ${BlendIntFont_HEADERS} ${BlendIntFont_SRC})

target_link_libraries(BlendIntFont ${LIBS})

install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/blendint/font" DESTINATION
    "include/blendint")
	
