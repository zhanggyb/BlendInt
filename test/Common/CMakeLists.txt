# build common code

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

add_library (Common Window.cpp)
target_link_libraries(Common BlendInt glfw ${GLFW_LIBRARIES} ${LIBS})
