#include "CMainWindow.hpp"
#include "graphics/CMainGraphicsWidget.hpp"
#include "loadingModule/CLoadingModule.hpp"

#include <QFileDialog>
#include <QPushButton>

#include <memory>

namespace
{
const auto TERRAIN_MODULE_TYPE {Types::eLoadingModule::Terrain}; //!< Enum for terrain loading module.
const auto TRACKS_MODULE_TYPE {Types::eLoadingModule::Tracks};   //!< Enum for track loading module.
}

CMainWindow::CMainWindow(QWidget* aParent)
  : QWidget(aParent)
{
    mUi.setupUi(this);
    mDataManager = std::make_unique<CDataManager>();

    QString DefaultFolderPath {mUi.FolderPathDisplay->text()};
    mDataManager->SetFolderPath(DefaultFolderPath);

    CreateLoadingModules();
    BindActions();
}

void CMainWindow::CreateLoadingModules()
{
    mLoadingModulesMap[TERRAIN_MODULE_TYPE] = std::make_unique<CLoadingModule>(TERRAIN_MODULE_TYPE, *mDataManager, *mUi.StatusBar);
    mLoadingModulesMap[TRACKS_MODULE_TYPE] = std::make_unique<CLoadingModule>(TRACKS_MODULE_TYPE, *mDataManager, *mUi.StatusBar);
}

void CMainWindow::BindActions()
{
    connect(mUi.LoadTerrainBtn, &QPushButton::clicked, this, &CMainWindow::LoadTerrainButtonPressed);
    connect(mUi.LoadTracksBtn, &QPushButton::clicked, this, &CMainWindow::LoadTracksButtonPressed);
    connect(mUi.CancelBtn, &QPushButton::clicked, this, &CMainWindow::CancelLoadButtonPressed);
    connect(mUi.OpenFolderBtn, &QPushButton::clicked, this, &CMainWindow::FolderButtonPressed);

    connect(mLoadingModulesMap[TERRAIN_MODULE_TYPE].get(), &CLoadingModule::FinishedSignal, this, &CMainWindow::LoadingModuleFinished);
    connect(mLoadingModulesMap[TRACKS_MODULE_TYPE].get(), &CLoadingModule::FinishedSignal, this, &CMainWindow::LoadingModuleFinished);

    connect(mLoadingModulesMap[TERRAIN_MODULE_TYPE].get(), &CLoadingModule::LoadInterrupted, this, &CMainWindow::OnLoadInterrupted);
    connect(mLoadingModulesMap[TRACKS_MODULE_TYPE].get(), &CLoadingModule::LoadInterrupted, this, &CMainWindow::OnLoadInterrupted);
}

void CMainWindow::FolderButtonPressed()
{
    QString AreaFolderPath {QFileDialog::getExistingDirectory(this, tr("Open Directory"), "", QFileDialog::ShowDirsOnly)};

    if (!AreaFolderPath.isEmpty())
    {
        mUi.FolderPathDisplay->setText(AreaFolderPath);
        mDataManager->SetFolderPath(AreaFolderPath);
    }
}

void CMainWindow::LoadTerrainButtonPressed()
{
    SetButtonsEnabled(eButtonsEnabledLayout::Loading);
    const auto& TerrainLoadingModule {mLoadingModulesMap.at(TERRAIN_MODULE_TYPE)};
    TerrainLoadingModule->LaunchLoadingModule();
}

void CMainWindow::LoadTracksButtonPressed()
{
    const auto& TerrainLoadingModule {mLoadingModulesMap.at(TERRAIN_MODULE_TYPE)};
    const auto& TracksLoadingModule {mLoadingModulesMap.at(TRACKS_MODULE_TYPE)};

    if (!TerrainLoadingModule->IsLoaded())
    {
        const std::string Message {"WARNING: Terrain must be loaded before tracks can be loaded!"};
        mUi.StatusBar->showMessage(Message.c_str(), 5000);
    }
    else
    {
        SetButtonsEnabled(eButtonsEnabledLayout::Loading);
        TracksLoadingModule->LaunchLoadingModule();
    }
}

void CMainWindow::CancelLoadButtonPressed()
{
    SetButtonsEnabled(eButtonsEnabledLayout::Canceling);

    const auto& TerrainLoadingModule {mLoadingModulesMap.at(TERRAIN_MODULE_TYPE)};
    const auto& TracksLoadingModule {mLoadingModulesMap.at(TRACKS_MODULE_TYPE)};

    if (TerrainLoadingModule->IsLoading())
    {
        TerrainLoadingModule->LaunchCancelLoad();
        const std::string Message {"Canceling terrain load..."};
        mUi.StatusBar->showMessage(Message.c_str());
    }
    else if (TracksLoadingModule->IsLoading())
    {
        TracksLoadingModule->LaunchCancelLoad();
        const std::string Message {"Canceling tracks load..."};
        mUi.StatusBar->showMessage(Message.c_str());
    }
}

void CMainWindow::OnLoadInterrupted()
{
    SetButtonsEnabled(eButtonsEnabledLayout::Rest);
}

void CMainWindow::LoadingModuleFinished(Types::eLoadingModule aModule)
{
    if (aModule == TERRAIN_MODULE_TYPE)
    {
        mUi.MainGraphics->LoadModel(mDataManager->GetTerrain());
    }
    else if (aModule == TRACKS_MODULE_TYPE)
    {
        mUi.MainGraphics->LoadTexture(mDataManager->GetTexture());
    }

    SetButtonsEnabled(eButtonsEnabledLayout::Rest);
}

void CMainWindow::SetButtonsEnabled(eButtonsEnabledLayout aLayout)
{
    std::vector<bool> ButtonsEnabled(4, false);

    switch (aLayout)
    {
        case eButtonsEnabledLayout::Rest:
        {
            ButtonsEnabled.clear();
            ButtonsEnabled = {true, true, true, false};
            break;
        }
        case eButtonsEnabledLayout::Loading:
        {
            ButtonsEnabled.clear();
            ButtonsEnabled = {false, false, false, true};
            break;
        }
        case eButtonsEnabledLayout::Canceling:
        {
            ButtonsEnabled.clear();
            ButtonsEnabled = {false, false, false, false};
            break;
        }
        default:
        {
            break;
        }
    }

    // TODO: HK-40. This could be turned into an enum.
    mUi.OpenFolderBtn->setEnabled(ButtonsEnabled.at(0U));
    mUi.LoadTracksBtn->setEnabled(ButtonsEnabled.at(1U));
    mUi.LoadTerrainBtn->setEnabled(ButtonsEnabled.at(2U));
    mUi.CancelBtn->setEnabled(ButtonsEnabled.at(3U));
}
