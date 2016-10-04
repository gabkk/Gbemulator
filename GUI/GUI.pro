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
    ../srcs/Registers.cpp \
    debugwindow.cpp \
    ../srcs/Cartridge.cpp \
    ../srcs/Cpu.cpp \
    ../srcs/Gb.cpp \
    ../srcs/Memory.cpp \
    mainwindow.cpp \
    hexspinbox.cpp

HEADERS  += \
    ../includes/Registers.class.hpp \
    debugwindow.h \
    ../includes/Cartridge.class.hpp \
    ../includes/Cpu.class.hpp \
    ../includes/Gb.class.hpp \
    ../includes/Gbmu.class.hpp \
    ../includes/Memory.class.hpp \
    mainwindow.h \
    hexspinbox.h

FORMS    += \
    debugwindow.ui \
    mainwindow.ui

DISTFILES += \
    README

RESOURCES += \
    resources.qrc
