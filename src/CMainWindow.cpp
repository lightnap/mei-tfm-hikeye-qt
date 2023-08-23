#include "CMainWindow.hpp"
#include "CLoadingModule.hpp"
#include "CMainGraphicsWidget.hpp"

#include <QPushButton>
#include <iostream>

namespace
{
const auto TERRAIN_MODULE_TYPE {Types::eLoadingModule::Terrain}; //!< Enum for terrain loading module.
const auto TRACKS_MODULE_TYPE {Types::eLoadingModule::Tracks};   //!< Enum for track loading module.
}

CMainWindow::CMainWindow(QWidget* aParent)
  : QWidget(aParent)
{
    mUi.setupUi(this);

    CreateLoadingModules();
    BindActions();
}

void CMainWindow::CreateLoadingModules()
{
    mLoadingModulesMap[TERRAIN_MODULE_TYPE] = std::make_unique<CLoadingModule>(TERRAIN_MODULE_TYPE, *mUi.StatusBar);
    mLoadingModulesMap[TRACKS_MODULE_TYPE] = std::make_unique<CLoadingModule>(TRACKS_MODULE_TYPE, *mUi.StatusBar);
}

void CMainWindow::BindActions()
{
    connect(mUi.LoadTerrainBtn, &QPushButton::clicked, this, &CMainWindow::LoadTerrain);
    connect(mUi.LoadTracksBtn, &QPushButton::clicked, this, &CMainWindow::LoadTracks);

    connect(mLoadingModulesMap[TERRAIN_MODULE_TYPE].get(), &CLoadingModule::FinishedSignal, this, &CMainWindow::LoadingModuleFinished);
    connect(mLoadingModulesMap[TRACKS_MODULE_TYPE].get(), &CLoadingModule::FinishedSignal, this, &CMainWindow::LoadingModuleFinished);

    void LoadingModuleFinished(Types::eLoadingModule aModule);
}

void CMainWindow::LoadTerrain()
{
    const auto& TerrainLoadingModule {mLoadingModulesMap.at(TERRAIN_MODULE_TYPE)};
    const auto& TracksLoadingModule {mLoadingModulesMap.at(TRACKS_MODULE_TYPE)};

    if (TerrainLoadingModule->IsLoading())
    {
        TerrainLoadingModule->CancelLoad();
    }

    if (TracksLoadingModule->IsLoading())
    {
        TracksLoadingModule->CancelLoad();
    }

    TerrainLoadingModule->LaunchLoader();
}

void CMainWindow::LoadTracks()
{
    const auto& TerrainLoadingModule {mLoadingModulesMap.at(TERRAIN_MODULE_TYPE)};
    const auto& TracksLoadingModule {mLoadingModulesMap.at(TRACKS_MODULE_TYPE)};

    if (!TerrainLoadingModule->IsLoaded())
    {
        // Print warning message
        std::string Message {"WARNING: Terrain must be loaded before tracks can be loaded!"};
        mUi.StatusBar->showMessage(Message.c_str(), 5000);
        return;
    }

    if (TracksLoadingModule->IsLoading())
    {
        TracksLoadingModule->CancelLoad();
    }

    TracksLoadingModule->LaunchLoader();
}

void CMainWindow::LoadingModuleFinished(Types::eLoadingModule aModule)
{
    if (aModule == TERRAIN_MODULE_TYPE)
    {
        mUi.MainGraphics->LoadModel();
    }
    else if (aModule == TRACKS_MODULE_TYPE)
    {
        mUi.MainGraphics->LoadTexture();
    }
}
