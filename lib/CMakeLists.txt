# CMake file for BlendInt project
#

file(GLOB core_headers "${PROJECT_SOURCE_DIR}/include/blendint/core/*.hpp")
file(GLOB core_sources "core/*.cpp")

file(GLOB cppevent_headers "${PROJECT_SOURCE_DIR}/include/blendint/cppevent/*.hpp")
file(GLOB cppevent_sources "cppevent/*.cpp")

file(GLOB font_headers "${PROJECT_SOURCE_DIR}/include/blendint/font/*.hpp")
file(GLOB font_sources "font/*.cpp")

file(GLOB gui_headers "${PROJECT_SOURCE_DIR}/include/blendint/gui/*.hpp")
file(GLOB gui_sources "gui/*.cpp")

file(GLOB opengl_headers "${PROJECT_SOURCE_DIR}/include/blendint/opengl/*.hpp")
file(GLOB opengl_sources "opengl/*.cpp")

file(GLOB stock_headers "${PROJECT_SOURCE_DIR}/include/blendint/stock/*.hpp")
file(GLOB stock_sources "stock/*.cpp")

if(BUILD_STATIC_LIBRARY)
  add_library (blendint
    ${core_headers}
    ${core_sources}
    ${cppevent_headers}
    ${cppevent_sources}
    ${font_headers}
    ${font_sources}
    ${gui_headers}
    ${gui_sources}
    ${opengl_headers}
    ${opengl_sources}
    ${stock_headers}
    ${stock_sources}
    )
else()
  add_library (blendint SHARED
    ${core_headers}
    ${core_sources}
    ${cppevent_headers}
    ${cppevent_sources}
    ${font_headers}
    ${font_sources}
    ${gui_headers}
    ${gui_sources}
    ${opengl_headers}
    ${opengl_sources}
    ${stock_headers}
    ${stock_sources}
    )
endif()

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

if(NOT DEFINED BUILD_STATIC_LIBRARY)
  set_target_properties(blendint PROPERTIES VERSION 1 SOVERSION 1)
endif()

target_link_libraries(blendint ${LIBS})

install(TARGETS blendint DESTINATION lib)

install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/blendint/core"
  DESTINATION "include/blendint")

install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/blendint/cppevent"
  DESTINATION "include/blendint")

install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/blendint/font"
  DESTINATION "include/blendint")
	
install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/blendint/gui"
  DESTINATION "include/blendint")

install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/blendint/opengl"
  DESTINATION "include/blendint")

install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/blendint/stock"
  DESTINATION "include/blendint")

