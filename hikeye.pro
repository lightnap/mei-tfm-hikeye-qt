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
    src/common \
    src/dataStructures \
    src/graphics \
    src/loadingModule \
    src/loadingModule/resourceLoader \
    src/loadingModule/resourceLoaderFactory \

HEADERS += \
    src/CMainWindow.hpp \
    src/graphics/CMainGraphicsWidget.hpp \
    src/graphics/CCamera.hpp \
    src/common/Types.hpp \
    src/common/Math.hpp \
    src/dataStructures/CDataManager.hpp \
    src/dataStructures/CConfigs.hpp \
    src/dataStructures/SHeightMap.hpp \
    src/dataStructures/STerrain.hpp \
    src/dataStructures/SGroundTruth.hpp \
    src/loadingModule/CLoadingModule.hpp \
    src/loadingModule/resourceLoader/CResourceLoader.hpp \
    src/loadingModule/resourceLoader/CGroundTruthResourceLoader.hpp \
    src/loadingModule/resourceLoader/CHeightMapResourceLoader.hpp \
    src/loadingModule/resourceLoader/CMatchesResourceLoader.hpp \
    src/loadingModule/resourceLoader/CQueriesResourceLoader.hpp \
    src/loadingModule/resourceLoader/CTerrainResourceLoader.hpp \
    src/loadingModule/resourceLoader/CTextureResourceLoader.hpp \
    src/loadingModule/resourceLoaderFactory/CResourceLoaderFactory.hpp \
    src/loadingModule/resourceLoaderFactory/IConcreteResourceLoaderFactory.hpp \

SOURCES += \
    src/main.cpp \
    src/CMainWindow.cpp \
    src/graphics/CMainGraphicsWidget.cpp \
    src/graphics/CCamera.cpp \
    src/common/Types.cpp \
    src/common/Math.cpp \
    src/dataStructures/CDataManager.cpp \
    src/dataStructures/STerrain.cpp \
    src/dataStructures/SHeightMap.cpp \
    src/dataStructures/SGroundTruth.cpp \
    src/loadingModule/CLoadingModule.cpp \
    src/loadingModule/resourceLoader/CResourceLoader.cpp \
    src/loadingModule/resourceLoader/CGroundTruthResourceLoader.cpp \
    src/loadingModule/resourceLoader/CHeightMapResourceLoader.cpp \
    src/loadingModule/resourceLoader/CMatchesResourceLoader.cpp \
    src/loadingModule/resourceLoader/CQueriesResourceLoader.cpp \
    src/loadingModule/resourceLoader/CTerrainResourceLoader.cpp \
    src/loadingModule/resourceLoader/CTextureResourceLoader.cpp \
    src/loadingModule/resourceLoaderFactory/CResourceLoaderFactory.cpp \
    src/loadingModule/resourceLoaderFactory/IConcreteResourceLoaderFactory.cpp \

FORMS += \
    src/CMainWindow.ui

#LIBS
