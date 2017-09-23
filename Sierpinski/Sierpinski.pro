#-------------------------------------------------
#
# Project created by QtCreator 2013-09-12T03:27:41
#
#-------------------------------------------------

QT       += core gui opengl
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Sierpinski
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    gldisplay.cpp \
    gasket.cpp \
    face.cpp \
    point3d.cpp \
    vec3.cpp \
    iterators.cpp \
    triangulation.cpp

HEADERS  += mainwindow.h \
    gldisplay.h \
    gasket.h \
    face.h \
    point3d.h \
    vec3.h \
    iterators.h \
    triangulation.h

FORMS    += mainwindow.ui
