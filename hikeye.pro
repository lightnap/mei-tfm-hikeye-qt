TEMPLATE = app
QT += core gui opengl

CONFIG += c++14
CONFIG += debug_and_release

MOC_DIR     = moc
OBJECTS_DIR = obj
UI_DIR      = ui
RCC_DIR     = ./

DESTDIR = ../bin

INCLUDEPATH += \
    src \
    src/loadingModule \
    src/loadingModule/resourceLoader \
    src/loadingModule/resourceLoaderFactory \

HEADERS += \
    src/Types.hpp \
    src/CMainWindow.hpp \
    src/CMainGraphicsWidget.hpp \
    src/loadingModule/CLoadingModule.hpp \
    src/loadingModule/resourceLoader/CResourceLoader.hpp \
    src/loadingModule/resourceLoaderFactory/CResourceLoaderFactory.hpp \
    src/loadingModule/resourceLoaderFactory/IConcreteResourceLoaderFactory.hpp \

SOURCES += \
    src/main.cpp \
    src/Types.cpp \
    src/CMainWindow.cpp \
    src/CMainGraphicsWidget.cpp \
    src/loadingModule/CLoadingModule.cpp \
    src/loadingModule/resourceLoader/CResourceLoader.cpp \
    src/loadingModule/resourceLoaderFactory/CResourceLoaderFactory.cpp \
    src/loadingModule/resourceLoaderFactory/IConcreteResourceLoaderFactory.cpp \

FORMS += \
    src/CMainWindow.ui

#LIBS
