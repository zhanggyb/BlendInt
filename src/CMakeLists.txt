# CMake file for BIL project
#

include_directories(${BIL_SOURCE_DIR}/include)
include_directories(${BIL_SOURCE_DIR}/include/BIL)
include_directories(${BIL_SOURCE_DIR}/extern/CppEvents/src)

set(LIBBIL_SRC AbstractButton.cpp Application.cpp BasicObject.cpp Container.cpp Controller.cpp Drawable.cpp Font.cpp FontFace.cpp FontManager.cpp GLDraw.cpp Label.cpp Layout.cpp RollingTriangle.cpp TextBuffer.cpp Theme.cpp VertexBuffer.cpp Window.cpp)

add_library (BIL SHARED ${LIBBIL_SRC})

set_target_properties(BIL PROPERTIES VERSION 1 SOVERSION 1)
