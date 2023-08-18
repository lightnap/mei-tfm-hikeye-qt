TEMPLATE = app
QT += core gui opengl

CONFIG += c++14
CONFIG += debug_and_release

INCLUDEPATH += src

MOC_DIR     = moc
OBJECTS_DIR = obj
UI_DIR      = ui
RCC_DIR     = ./

DESTDIR = ../bin

FORMS += \
    src/CMainWindow.ui

HEADERS += \
    src/CMainWindow.hpp \
    src/CMainGraphicsWidget.hpp

SOURCES += \
    src/main.cpp \
    src/CMainWindow.cpp \
    src/CMainGraphicsWidget.cpp


#LIBS
