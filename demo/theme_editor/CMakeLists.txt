# CMake file for BlendInt project
#

# Find the QtWidgets library
find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5OpenGL)

set(LIBS ${LIBS} Qt5::Core Qt5::Widgets Qt5::OpenGL)

file(GLOB sourcefiles "*/*.cpp")
list(APPEND theme_editor_SRC ${sourcefiles})
file(GLOB sourcefiles "*.cpp")
list(APPEND theme_editor_SRC ${sourcefiles})
file(GLOB headerfiles "*.hpp")
list(APPEND theme_editor_SRC ${headfiles})
file(GLOB headerfiles "*.h")
list(APPEND theme_editor_SRC ${headfiles})

# Be sure to add ui files
#file(GLOB uifiles "*.ui")
#list(APPEND theme_editor_SRC ${uifiles})

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

#qt5_wrap_ui(ui_mainwindow.h mainwindow.ui)

#qt5_generate_moc(mainwindow.h moc_mainwindow.cpp)
#qt5_generate_moc(glwindow.h moc_glwindow.cpp)

add_executable(theme_editor ${theme_editor_SRC})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
target_link_libraries(theme_editor BlendInt ${LIBS})

