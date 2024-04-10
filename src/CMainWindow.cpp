#include "CMainWindow.hpp"
#include "graphics/CMainGraphicsWidget.hpp"
#include "loadingModule/CLoadingModule.hpp"

#include "QtRangeSlider/RangeSlider.h"

#include <QApplication>
#include <QFileDialog>
#include <QKeyEvent>
#include <QPushButton>
#include <QSpinBox>

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
    InitWidgets();
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

    connect(mUi.DoubleSlider, &RangeSlider::lowerValueChanged, this, &CMainWindow::RangeSliderLowerValueChanged);
    connect(mUi.DoubleSlider, &RangeSlider::upperValueChanged, this, &CMainWindow::RangeSliderUpperValueChanged);
    connect(mUi.DoubleSlider, &RangeSlider::slidingFinished, this, &CMainWindow::RangeSliderStoppedSlinding);
    connect(mUi.DoubleSliderMin, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &CMainWindow::MinRangeSpinBoxChanged);
    connect(mUi.DoubleSliderMax, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &CMainWindow::MaxRangeSpinBoxChanged);

    connect(mLoadingModulesMap[TERRAIN_MODULE_TYPE].get(), &CLoadingModule::FinishedSignal, this, &CMainWindow::LoadingModuleFinished);
    connect(mLoadingModulesMap[TRACKS_MODULE_TYPE].get(), &CLoadingModule::FinishedSignal, this, &CMainWindow::LoadingModuleFinished);
    connect(mLoadingModulesMap[SAVE_IMAGE_MODULE_TYPE].get(), &CLoadingModule::FinishedSignal, this, &CMainWindow::LoadingModuleFinished);

    connect(mLoadingModulesMap[TERRAIN_MODULE_TYPE].get(), &CLoadingModule::LoadInterrupted, this, &CMainWindow::OnLoadInterrupted);
    connect(mLoadingModulesMap[TRACKS_MODULE_TYPE].get(), &CLoadingModule::LoadInterrupted, this, &CMainWindow::OnLoadInterrupted);
    connect(mLoadingModulesMap[SAVE_IMAGE_MODULE_TYPE].get(), &CLoadingModule::LoadInterrupted, this, &CMainWindow::OnLoadInterrupted);
}

void CMainWindow::InitWidgets()
{
    mUi.DoubleSliderMax->setValue(mUi.DoubleSlider->GetUpperValue());
    mUi.DoubleSliderMin->setValue(mUi.DoubleSlider->GetLowerValue());

    mDataManager->SetPaintRangeCurrentLower(mUi.DoubleSlider->GetLowerValue());
    mDataManager->SetPaintRangeCurrentUpper(mUi.DoubleSlider->GetUpperValue());
    mDataManager->SetPaintRangeMax(mUi.DoubleSlider->GetMaximun());
    mDataManager->SetPaintRangeMin(mUi.DoubleSlider->GetMinimun());
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
    // Painting strategy.
    if (mUi.NoStrategyButton->isChecked())
    {
        mDataManager->SetPaintStrategy(Types::ePaintStrategy::None);
    }
    else if (mUi.CrossingsStrategyButton->isChecked())
    {
        mDataManager->SetPaintStrategy(Types::ePaintStrategy::CountCrossings);
    }
    else if (mUi.CrossingsPerMatchButton->isChecked())
    {
        mDataManager->SetPaintStrategy(Types::ePaintStrategy::CountCrossingsPerMatch);
    }
    else if (mUi.DirectionStrategyButton->isChecked())
    {
        mDataManager->SetPaintStrategy(Types::ePaintStrategy::Directions);
    }
    else if (mUi.DirectionStrategyButton->isChecked())
    {
        mDataManager->SetPaintStrategy(Types::ePaintStrategy::Speed);
    }

    Types::sDateFilter DateFilter;

    // Days of the week.
    if (!mUi.checkBoxMonday->isChecked())
    {
        DateFilter.FilteredDays[Qt::Monday] = true;
    }
    if (!mUi.checkBoxTuesday->isChecked())
    {
        DateFilter.FilteredDays[Qt::Tuesday] = true;
    }
    if (!mUi.checkBoxWednesday->isChecked())
    {
        DateFilter.FilteredDays[Qt::Wednesday] = true;
    }
    if (!mUi.checkBoxThursday->isChecked())
    {
        DateFilter.FilteredDays[Qt::Thursday] = true;
    }
    if (!mUi.checkBoxFriday->isChecked())
    {
        DateFilter.FilteredDays[Qt::Friday] = true;
    }
    if (!mUi.checkBoxSaturday->isChecked())
    {
        DateFilter.FilteredDays[Qt::Saturday] = true;
    }
    if (!mUi.checkBoxSunday->isChecked())
    {
        DateFilter.FilteredDays[Qt::Sunday] = true;
    }

    // Months.
    if (!mUi.checkBoxJanuary->isChecked())
    {
        DateFilter.FilteredMonths[1] = true;
    }
    if (!mUi.checkBoxFebruary->isChecked())
    {
        DateFilter.FilteredMonths[2] = true;
    }
    if (!mUi.checkBoxMarch->isChecked())
    {
        DateFilter.FilteredMonths[3] = true;
    }
    if (!mUi.checkBoxApril->isChecked())
    {
        DateFilter.FilteredMonths[4] = true;
    }
    if (!mUi.checkBoxMay->isChecked())
    {
        DateFilter.FilteredMonths[5] = true;
    }
    if (!mUi.checkBoxJune->isChecked())
    {
        DateFilter.FilteredMonths[6] = true;
    }
    if (!mUi.checkBoxJuly->isChecked())
    {
        DateFilter.FilteredMonths[7] = true;
    }
    if (!mUi.checkBoxAugust->isChecked())
    {
        DateFilter.FilteredMonths[8] = true;
    }
    if (!mUi.checkBoxSeptember->isChecked())
    {
        DateFilter.FilteredMonths[9] = true;
    }
    if (!mUi.checkBoxOctober->isChecked())
    {
        DateFilter.FilteredMonths[10] = true;
    }
    if (!mUi.checkBoxNovember->isChecked())
    {
        DateFilter.FilteredMonths[11] = true;
    }
    if (!mUi.checkBoxDecember->isChecked())
    {
        DateFilter.FilteredMonths[12] = true;
    }

    mDataManager->SetDateFilter(DateFilter);

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
            mUi.MainGraphics->LoadTexture(mDataManager->GetTerrainTexture());
            SetButtonsEnabled(eButtonsEnabledLayout::Rest);
            break;
        }
        case TRACKS_MODULE_TYPE:
        {
            // std::cout << "Finished track loading module" << std::endl;
            // std::cout << "Current slider upper" << mUi.DoubleSlider->GetUpperValue() << std::endl;
            // std::cout << "Current slider max" << mUi.DoubleSlider->GetMaximun() << std::endl;
            if (mUi.DoubleSlider->GetMinimun() != mDataManager->GetPaintRangeMin())
            {
                mUi.DoubleSlider->SetMinimum(mDataManager->GetPaintRangeMin());
            }
            if (mUi.DoubleSlider->GetMaximun() != mDataManager->GetPaintRangeMax())
            {
                mUi.DoubleSlider->SetMaximum(mDataManager->GetPaintRangeMax());
            }
            // mUi.DoubleSlider->SetUpperValue(mDataManager->GetPaintRangeCurrentUpper());
            // mUi.DoubleSlider->SetLowerValue(mDataManager->GetPaintRangeCurrentLower());
            // std::cout << "Set things to things" << std::endl;
            // std::cout << "Current slider upper" << mUi.DoubleSlider->GetUpperValue() << std::endl;
            // std::cout << "Current slider max" << mUi.DoubleSlider->GetMaximun() << std::endl;
            // std::cout << "DataManager values: Max" << mDataManager->GetPaintRangeMax() << " Upper: " << mDataManager->GetPaintRangeCurrentUpper() << std::endl;

            mUi.MainGraphics->LoadTexture(mDataManager->GetTracksTexture());
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

void CMainWindow::RangeSliderUpperValueChanged(int aNewHigh)
{
    mUi.DoubleSliderMax->setValue(aNewHigh);
    mDataManager->SetPaintRangeCurrentUpper(aNewHigh);
}

void CMainWindow::RangeSliderLowerValueChanged(int aNewLow)
{
    mUi.DoubleSliderMin->setValue(aNewLow);
    mDataManager->SetPaintRangeCurrentLower(aNewLow);
}

void CMainWindow::RangeSliderStoppedSlinding()
{
    // TODO: Nothing for now.
}

void CMainWindow::MinRangeSpinBoxChanged(int aNewValue)
{
    const int NewLowerValue {std::min(mUi.DoubleSlider->GetUpperValue(), aNewValue)};
    mUi.DoubleSlider->SetLowerValue(NewLowerValue);
    mUi.DoubleSliderMin->setValue(NewLowerValue);
}

void CMainWindow::MaxRangeSpinBoxChanged(int aNewValue)
{
    const int NewUpperValue {std::max(mUi.DoubleSlider->GetLowerValue(), aNewValue)};
    mUi.DoubleSlider->SetUpperValue(NewUpperValue);
    mUi.DoubleSliderMax->setValue(NewUpperValue);
}
