# CMake file for BlendInt project
#
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

file(GLOB CppEvents_HEADERS "${PROJECT_SOURCE_DIR}/intern/blendint/include/cppevents/*.hpp")
file(GLOB CppEvents_SRC "*.cpp")

add_library (CppEvents ${CppEvents_HEADERS} ${CppEvents_SRC})

install(DIRECTORY "${PROJECT_SOURCE_DIR}/intern/blendint/include/cppevents" DESTINATION
    "include/blendint")
