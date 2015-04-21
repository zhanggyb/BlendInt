# CMake file for BlendInt project
#

file(GLOB BlendIntOpenGL_HEADERS "${PROJECT_SOURCE_DIR}/include/blendint/opengl/*.hpp")
file(GLOB BlendIntOpenGL_SRC "*.cpp")

add_library (BlendIntOpenGL ${BlendIntOpenGL_HEADERS} ${BlendIntOpenGL_SRC})

target_link_libraries(BlendIntOpenGL ${LIBS})

install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/blendint/opengl" DESTINATION
    "include/blendint")
