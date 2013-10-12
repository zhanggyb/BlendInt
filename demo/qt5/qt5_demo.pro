#-------------------------------------------------
#
# Project created by QtCreator 2013-09-03T15:13:38
#
#-------------------------------------------------

QT       += core gui opengl

INCLUDEPATH = ../../../include
INCLUDEPATH += ../../../include/BlendInt
INCLUDEPATH += ../../../extern/CppEvents/src

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt5_demo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glwindow.cpp

HEADERS  += mainwindow.h \
    glwindow.h

FORMS    += mainwindow.ui
