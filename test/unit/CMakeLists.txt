# CMake file for BIL project
#

file(GLOB runtest_SRC "*/*.cpp")
list(APPEND runtest_SRC "runtest.cpp")

list(APPEND LIBS cppunit)

add_executable(runtest ${runtest_SRC})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
target_link_libraries(runtest BIL ${LIBS})
