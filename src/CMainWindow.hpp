#ifndef C_MAIN_WINDOW_H
#define C_MAIN_WINDOW_H

#include "common/Types.hpp"
#include "dataStructures/CDataManager.hpp"
#include "loadingModule/CLoadingModule.hpp"
#include "ui_CMainWindow.h"

#include <map>
#include <memory>

/**
 * Class that represents the main window of our GUI application.
 */
class CMainWindow : public QWidget
{
    Q_OBJECT

  public:
    /**
     * @brief Constructor.
     * @param aParent Parent widget in hierarchy.
     */
    CMainWindow(QWidget* aParent = 0);

  public slots:

    void RangeSliderLowerValueChanged(int aNewLow);
    void RangeSliderUpperValueChanged(int aNewHigh);
    void RangeSliderStoppedSlinding();
    void MinRangeSpinBoxChanged(int aNewValue);
    void MaxRangeSpinBoxChanged(int aNewValue);

    /**
     * @brief Slots for reacting to open folder button press.
     */
    void FolderButtonPressed();

    /**
     * @brief Slot for reacting to load terrain button press.
     */
    void LoadTerrainButtonPressed();

    /**
     * @brief Slot for reacting to load tracks button press.
     */
    void LoadTracksButtonPressed();

    /**
     * @brief Slot for reacting to cancel load button press.
     */
    void CancelLoadButtonPressed();

    /**
     * @brief Slot for reacting to save image button press.
     */
    void SaveImageButtonPressed();

    /**
     * @brief Gets called when a loading module has finished.
     * @param aModule Type of the module that has finished loading.
     */
    void LoadingModuleFinished(Types::eLoadingModule aModule);

    /**
     * @brief Gets called when a loading module has interrupted its loading.
     */
    void OnLoadInterrupted();

  protected:
    /**
     * @brief Manages user keyboard input events.
     * @param apEvent: Event containing the key press info.
     */
    void keyPressEvent(QKeyEvent* apEvent) override;

  private:
    /**
     * Enumdescribing the possible states of the buttons in the layout.
     */
    enum class eButtonsEnabledLayout
    {
        Rest = 0,  //!< App is at rest.
        Loading,   //!< App is performing a load process.
        Canceling, //!< App is performing a canceling process.
        Loaded,    //!< App has finished loaded process.
        Size,      //!< Size of this enum.
    };

    /**
     * @brief Binds all slots and signals of this window.
     */
    void BindActions();

    /**
     * @brief Creates loading modules.
     */
    void CreateLoadingModules();

    void InitWidgets();

    /**
     * @brief Sets the app buttons enabled/disabled, based on a layout.
     * @param aLayout: Layout we use to determine which buttons are enabled.
     */
    void SetButtonsEnabled(eButtonsEnabledLayout aLayout);

  private:
    using tLoadingModulesMap = std::map<Types::eLoadingModule, std::unique_ptr<CLoadingModule>>; //!< Type for relating loading modules and their type.

    tLoadingModulesMap            mLoadingModulesMap; //!< Map with all loaging modules.
    Ui::CMainWindow               mUi;                //!< Represents the Ui of this form.
    std::unique_ptr<CDataManager> mDataManager;       //!< Data manager.
};

#endif // C_MAIN_WINDOW_H
