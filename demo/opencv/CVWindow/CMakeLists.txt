# build common code

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

if(NOT WITH_BUILTIN_GLFW3)
    if(OS_APPLE)
        set(LIBS ${LIBS} glfw3)
    else()
        set(LIBS ${LIBS} glfw)
    endif()
endif()

add_library (CVWindow Window.cpp CVCursor.cpp)
target_link_libraries(CVWindow BlendInt ${LIBS})

