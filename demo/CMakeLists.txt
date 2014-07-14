# CMake file for BIL project
#

if(WITH_GLFW3_DEMO)
    add_subdirectory(glfw)
    add_subdirectory(scene)
    add_subdirectory(texteditor)
    add_subdirectory(theme_editor)
    if(ENABLE_OPENCV)
    add_subdirectory(imgproc)
    add_subdirectory(texbuf)
    endif()
endif()

if(WITH_QT5_DEMO)
    add_subdirectory(qt5)
    add_subdirectory(opencv)
endif()

if(WITH_GLUT_DEMO)
    add_subdirectory(glut)
endif()

