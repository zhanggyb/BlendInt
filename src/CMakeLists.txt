# CMake file for BIL project
#

file(GLOB libBIL_SRC "*.cpp")

add_library (BIL SHARED ${libBIL_SRC})

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set_target_properties(BIL PROPERTIES VERSION 1 SOVERSION 1)
target_link_libraries(BIL CppEvents ${LIBS})
