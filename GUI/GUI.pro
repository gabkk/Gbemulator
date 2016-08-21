#-------------------------------------------------
#
# Project created by QtCreator 2016-08-20T20:01:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ../srcs/Registers.cpp

HEADERS  += mainwindow.h \
    gbmu_gui.h \
    ../includes/Registers.class.hpp

FORMS    += mainwindow.ui
