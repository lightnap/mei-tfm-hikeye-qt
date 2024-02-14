#include "CMainWindow.hpp"
#include "graphics/CMainGraphicsWidget.hpp"
#include "loadingModule/CLoadingModule.hpp"

#include <QApplication>
#include <QFileDialog>
#include <QKeyEvent>
#include <QPushButton>

#include <iostream>
#include <memory>

namespace
{
    const auto TERRAIN_MODULE_TYPE {Types::eLoadingModule::Terrain};      //!< Enum for terrain loading module.
    const auto TRACKS_MODULE_TYPE {Types::eLoadingModule::Tracks};        //!< Enum for track loading module.
    const auto SAVE_IMAGE_MODULE_TYPE {Types::eLoadingModule::SaveImage}; //!< Enum for save image loading module.
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
    mLoadingModulesMap[SAVE_IMAGE_MODULE_TYPE] = std::make_unique<CLoadingModule>(SAVE_IMAGE_MODULE_TYPE, *mDataManager, *mUi.StatusBar);
}

void CMainWindow::BindActions()
{
    connect(mUi.LoadTerrainBtn, &QPushButton::clicked, this, &CMainWindow::LoadTerrainButtonPressed);
    connect(mUi.LoadTracksBtn, &QPushButton::clicked, this, &CMainWindow::LoadTracksButtonPressed);
    connect(mUi.CancelBtn, &QPushButton::clicked, this, &CMainWindow::CancelLoadButtonPressed);
    connect(mUi.OpenFolderBtn, &QPushButton::clicked, this, &CMainWindow::FolderButtonPressed);
    connect(mUi.SaveImageBtn, &QPushButton::clicked, this, &CMainWindow::SaveImageButtonPressed);

    connect(mLoadingModulesMap[TERRAIN_MODULE_TYPE].get(), &CLoadingModule::FinishedSignal, this, &CMainWindow::LoadingModuleFinished);
    connect(mLoadingModulesMap[TRACKS_MODULE_TYPE].get(), &CLoadingModule::FinishedSignal, this, &CMainWindow::LoadingModuleFinished);
    connect(mLoadingModulesMap[SAVE_IMAGE_MODULE_TYPE].get(), &CLoadingModule::FinishedSignal, this, &CMainWindow::LoadingModuleFinished);

    connect(mLoadingModulesMap[TERRAIN_MODULE_TYPE].get(), &CLoadingModule::LoadInterrupted, this, &CMainWindow::OnLoadInterrupted);
    connect(mLoadingModulesMap[TRACKS_MODULE_TYPE].get(), &CLoadingModule::LoadInterrupted, this, &CMainWindow::OnLoadInterrupted);
    connect(mLoadingModulesMap[SAVE_IMAGE_MODULE_TYPE].get(), &CLoadingModule::LoadInterrupted, this, &CMainWindow::OnLoadInterrupted);
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

void CMainWindow::SaveImageButtonPressed()
{
    SetButtonsEnabled(eButtonsEnabledLayout::Loading);
    mLoadingModulesMap.at(SAVE_IMAGE_MODULE_TYPE)->LaunchLoadingModule();
}

void CMainWindow::OnLoadInterrupted()
{
    SetButtonsEnabled(eButtonsEnabledLayout::Rest);
}

void CMainWindow::LoadingModuleFinished(Types::eLoadingModule aModule)
{
    switch (aModule)
    {
        case TERRAIN_MODULE_TYPE:
        {
            mUi.MainGraphics->LoadModel(mDataManager->GetTerrain());
            mUi.MainGraphics->LoadTexture(mDataManager->GetTexture());
            SetButtonsEnabled(eButtonsEnabledLayout::Rest);
            break;
        }
        case TRACKS_MODULE_TYPE:
        {
            mUi.MainGraphics->LoadTexture(mDataManager->GetTexture());
            SetButtonsEnabled(eButtonsEnabledLayout::Loaded);
            break;
        }
        case SAVE_IMAGE_MODULE_TYPE:
        {
            SetButtonsEnabled(eButtonsEnabledLayout::Loaded);
            const std::string Message {"Image successfully saved"};
            mUi.StatusBar->showMessage(Message.c_str(), 5000);

            break;
        }
        default:
        {
            break;
        }
    }
}

void CMainWindow::keyPressEvent(QKeyEvent* apEvent)
{
    if (apEvent->key() == Qt::Key_Escape)
    {
        QApplication::quit();
    }
}

void CMainWindow::SetButtonsEnabled(eButtonsEnabledLayout aLayout)
{
    std::vector<bool> ButtonsEnabled(5, false);

    switch (aLayout)
    {
        case eButtonsEnabledLayout::Rest:
        {
            ButtonsEnabled.clear();
            ButtonsEnabled = {true, true, true, false, false};
            break;
        }
        case eButtonsEnabledLayout::Loading:
        {
            ButtonsEnabled.clear();
            ButtonsEnabled = {false, false, false, true, false};
            break;
        }
        case eButtonsEnabledLayout::Canceling:
        {
            ButtonsEnabled.clear();
            ButtonsEnabled = {false, false, false, false, false};
            break;
        }
        case eButtonsEnabledLayout::Loaded:
        {
            ButtonsEnabled.clear();
            ButtonsEnabled = {true, true, true, false, true};
            break;
        }
        default:
        {
            break;
        }
    }

    // TODO: HK-40. This could be turned into an enum. Like instead of a vector,
    // a map from an enum (each entry representing one of the buttons) to a bool.
    mUi.OpenFolderBtn->setEnabled(ButtonsEnabled.at(0U));
    mUi.LoadTracksBtn->setEnabled(ButtonsEnabled.at(1U));
    mUi.LoadTerrainBtn->setEnabled(ButtonsEnabled.at(2U));
    mUi.CancelBtn->setEnabled(ButtonsEnabled.at(3U));
    mUi.SaveImageBtn->setEnabled(ButtonsEnabled.at(4U));
}
