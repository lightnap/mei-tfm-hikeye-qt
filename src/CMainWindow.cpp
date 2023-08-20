#include "CMainWindow.hpp"
#include "CLoadingModule.hpp"
#include "CMainGraphicsWidget.hpp"

#include <iostream>

namespace
{
const auto TERRAIN_MODULE_TYPE {CLoadingModule::eModuleType::Terrain}; //!< Enum for terrain loading module.
const auto TRACKS_MODULE_TYPE {CLoadingModule::eModuleType::Tracks};   //!< Enum for track loading module.
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
    connect(mUi.LoadTerrainBtn, SIGNAL(clicked()), this, SLOT(LoadTerrain()));
    connect(mUi.LoadTracksBtn, SIGNAL(clicked()), this, SLOT(LoadTracks()));
    connect(mLoadingModulesMap[TERRAIN_MODULE_TYPE].get(), SIGNAL(FinishedSignal(eModuleType aType)), this, SLOT(LoadingModuleFinished(CLoadingModule::eModuleType aType)));
    connect(mLoadingModulesMap[TRACKS_MODULE_TYPE].get(), SIGNAL(FinishedSignal(eModuleType aType)), this, SLOT(LoadingModuleFinished(CLoadingModule::eModuleType aType)));

    void LoadingModuleFinished(CLoadingModule::eModuleType aModule);
}

void CMainWindow::LoadTerrain()
{
    const auto& TerrainLoadingModule {mLoadingModulesMap[CLoadingModule::eModuleType::Terrain]};
    const auto& TracksLoadingModule {mLoadingModulesMap[CLoadingModule::eModuleType::Terrain]};

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
    const auto& TerrainLoadingModule {mLoadingModulesMap[CLoadingModule::eModuleType::Terrain]};
    const auto& TracksLoadingModule {mLoadingModulesMap[CLoadingModule::eModuleType::Terrain]};

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

void CMainWindow::LoadingModuleFinished(CLoadingModule::eModuleType aModule)
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
