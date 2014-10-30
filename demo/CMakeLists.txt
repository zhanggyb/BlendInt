# CMake file for BIL project
#

if(WITH_GLFW3_DEMO)
    add_subdirectory(glfw)
    #add_subdirectory(studio)
    #add_subdirectory(scene)
    #add_subdirectory(texteditor)
    #add_subdirectory(texbuf)
    #add_subdirectory(imgproc)
    #if(ENABLE_OPENCV)
#	add_subdirectory(cv)
#	add_subdirectory(opencv)
#    endif()
endif()

if(WITH_QT5_DEMO)
    add_subdirectory(theme_editor)
    add_subdirectory(qt5)
endif()

if(WITH_GLUT_DEMO)
    add_subdirectory(glut)
endif()

