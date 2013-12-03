# CMake file for BIL project
#

if(WITH_GLFW3_DEMO)
    add_subdirectory(glfw)
endif()

if(WITH_QT5_DEMO)
    add_subdirectory(qt5)
endif()

if(WITH_GLUT_DEMO)
    add_subdirectory(glut)
endif()

