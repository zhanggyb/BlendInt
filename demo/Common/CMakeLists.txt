# build common code

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

file(GLOB sourcefiles "*/*.cpp")
list(APPEND demo_common_SRC ${sourcefiles})
file(GLOB sourcefiles "*.cpp")
list(APPEND demo_common_SRC ${sourcefiles})
file(GLOB headerfiles "*.hpp")
list(APPEND demo_common_SRC ${headfiles})
file(GLOB headerfiles "*/*.hpp")
list(APPEND demo_common_SRC ${headfiles})

if(NOT WITH_BUILTIN_GLFW3)
    if(OS_APPLE)
        set(LIBS ${LIBS} glfw3)
    else()
        set(LIBS ${LIBS} glfw)
    endif()
endif()

add_library (DemoCommon ${demo_common_SRC})
target_link_libraries(DemoCommon BlendInt ${LIBS})

