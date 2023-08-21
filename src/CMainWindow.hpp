#ifndef C_MAIN_WINDOW_H
#define C_MAIN_WINDOW_H

#include "CLoadingModule.hpp"
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
    /**
     * @brief Begins the load terrain process.
     */
    void LoadTerrain();

    /**
     * @brief Begins the load tracks process.
     */
    void LoadTracks();

    /**
     * @brief Gets called when a loading module has finished.
     * @param aModule Type of the module that has finished loading.
     */
    void LoadingModuleFinished(CLoadingModule::eModuleType aModule);

  private:
    /**
     * @brief Binds all slots and signals of this window.
     */
    void BindActions();

    /**
     * @brief Creates loading modules.
     */
    void CreateLoadingModules();

  private:
    using tLoadingModulesMap = std::map<CLoadingModule::eModuleType, std::unique_ptr<CLoadingModule>>; //!< Type for relating loading modules and their type.

    tLoadingModulesMap mLoadingModulesMap; //!< Map with all loaging modules.
    Ui::CMainWindow    mUi;                //!< Represents the Ui of this form.
};

#endif // C_MAIN_WINDOW_H