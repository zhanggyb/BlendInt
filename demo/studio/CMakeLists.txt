# CMake file for BlendInt project
#

file(GLOB sourcefiles "*/*.cpp")
list(APPEND studio_SRC ${sourcefiles})
file(GLOB sourcefiles "*.cpp")
list(APPEND studio_SRC ${sourcefiles})
file(GLOB headerfiles "*.hpp")
list(APPEND studio_SRC ${headfiles})
file(GLOB headerfiles "*/*.hpp")
list(APPEND studio_SRC ${headfiles})

add_executable(studio ${studio_SRC})

if(NOT WITH_BUILTIN_GLFW3)
    if(OS_APPLE)
        set(LIBS ${LIBS} glfw3)
    else()
        set(LIBS ${LIBS} glfw)
    endif()
endif()

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
target_link_libraries(studio BlendInt DemoCommon ${LIBS})

