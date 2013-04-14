#-------------------------------------------------
#
# Project created by QtCreator 2013-04-14T15:46:14
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../
HEADERS += ../contour.h \
           ../contouralgorithms.h \
           ../DataStructures.h \
           ../geometryutils.h

SOURCES += test.cpp \
           ../contouralgorithms.cpp \
           ../contour.cpp \
           ../geometryutils.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"
