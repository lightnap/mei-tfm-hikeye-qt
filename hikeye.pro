TEMPLATE = app
QT += core gui opengl

CONFIG += c++14
CONFIG += debug_and_release

INCLUDEPATH += \
    src \
    src/loadingModule

MOC_DIR     = moc
OBJECTS_DIR = obj
UI_DIR      = ui
RCC_DIR     = ./

DESTDIR = ../bin

FORMS += \
    src/CMainWindow.ui

HEADERS += \
    src/Types.hpp \
    src/CMainWindow.hpp \
    src/CMainGraphicsWidget.hpp \
    src/loadingModule/CLoadingModule.hpp \
    src/loadingModule/CResourceLoader.hpp

SOURCES += \
    src/main.cpp \
    src/Types.cpp \
    src/CMainWindow.cpp \
    src/CMainGraphicsWidget.cpp \
    src/loadingModule/CLoadingModule.cpp \
    src/loadingModule/CResourceLoader.cpp

#LIBS
