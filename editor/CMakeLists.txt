# CMake file for BlendInt project
#

file(GLOB sourcefiles "*/*.cpp")
list(APPEND editor_SRC ${sourcefiles})
file(GLOB sourcefiles "*.cpp")
list(APPEND editor_SRC ${sourcefiles})

file(GLOB headerfiles "*.hpp")
list(APPEND editor_SRC ${headerfiles})
file(GLOB headerfiles "*/*.hpp")
list(APPEND editor_SRC ${headerfiles})

add_executable(editor ${editor_SRC})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
target_link_libraries(editor BlendInt ${LIBS})

if(CMAKE_GENERATOR STREQUAL "Xcode")
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    INSTALL(PROGRAMS ${PROJECT_BINARY_DIR}/bin/Debug/editor DESTINATION bin)
  else()
    INSTALL(PROGRAMS ${PROJECT_BINARY_DIR}/bin/Release/editor DESTINATION bin)
  endif()
else()
    INSTALL(PROGRAMS ${PROJECT_BINARY_DIR}/bin/editor DESTINATION bin)
endif()

