# CMake file for BlendInt project
#

file(GLOB BlendIntCore_HEADERS "${PROJECT_SOURCE_DIR}/include/blendint/core/*.hpp")
file(GLOB BlendIntCore_SRC "*.cpp")

add_library (BlendIntCore ${BlendIntCore_HEADERS} ${BlendIntCore_SRC})

target_link_libraries(BlendIntCore ${LIBS})

install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/blendint/core" DESTINATION
    "include/blendint")
