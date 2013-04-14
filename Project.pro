#-------------------------------------------------
#
# Project created by QtCreator 2013-03-02T16:28:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Project
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Algorithms.cpp \
    Rendering.cpp \
    contour.cpp

HEADERS  += mainwindow.h \
    Algorithms.h \
    DataStructures.h \
    Rendering.h \
    contour.h

FORMS    += mainwindow.ui
