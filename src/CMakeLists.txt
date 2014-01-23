# CMake file for BlendInt project
#

file(GLOB libBlendInt_SRC "*.cpp")

if(BUILD_STATIC_LIBRARY)
    add_library (BlendInt ${libBlendInt_SRC})
else()
    add_library (BlendInt SHARED ${libBlendInt_SRC})
endif()

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

if(NOT DEFINED BUILD_STATIC_LIBRARY)
    set_target_properties(BlendInt PROPERTIES VERSION 1 SOVERSION 1)
endif()

target_link_libraries(BlendInt CppEvents glfw ${GLFW_LIBRARIES} ${LIBS})
