# CMake file for BlendInt project
#

add_subdirectory(Core)
add_subdirectory(OpenGL)
add_subdirectory(Gui)
add_subdirectory(Stock)
add_subdirectory(CppEvents)

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

if(WITH_BUILTIN_GLFW3)
    set(LIBS ${LIBS} glfw ${GLFW_LIBRARIES})
endif()

set(MODULES BlendIntCore BlendIntOpenGL BlendIntGui BlendIntStock CppEvents)

target_link_libraries(BlendInt ${MODULES} ${LIBS})

install(TARGETS BlendInt DESTINATION lib)
