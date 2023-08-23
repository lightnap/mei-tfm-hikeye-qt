#ifndef C_LOADING_MODULE_H
#define C_LOADING_MODULE_H

#include "CResourceLoader.hpp"
#include "Types.hpp"

#include <QObject>
#include <QStatusBar>
#include <map>
#include <memory>
#include <string>
#include <vector>

/**
 * Class that represents a whole loading process.
 */
class CLoadingModule : public QObject
{

    Q_OBJECT

  public:
    /**
     * @brief Constructor.
     * @param aModuleType Type of the module we want to contruct.
     * @param aStatusBar Reference to CMainWindow's StatusBar.
     */
    CLoadingModule(Types::eLoadingModule aModuleType, QStatusBar& aStatusBar);

    /**
     * @brief Starts the loading process.
     */
    void LaunchLoader();

    /**
     * @brief Stops current loading process (if there is one).
     */
    void CancelLoad();

    /**
     * @brief Returns whether this module has started loading.
     * @return True if module is yet to be loaded, false othersise.
     */
    [[nodiscard]] bool IsUnLoaded() const;

    /**
     * @brief Returns whether this modules is currently loading.
     * @return True if module is loading, false otherwise.
     */
    [[nodiscard]] bool IsLoading() const;

    /**
     * @brief Returns whether this module is fully loaded.
     * @return True if module finished loading, false otherwise.
     */
    [[nodiscard]] bool IsLoaded() const;

  public slots:
    /**
     * @brief Gets called when a resource finishes loading.
     */
    void ResourceFinishedLoading();

  signals:
    /**
     * @brief Signal to signalize that the module has finished loading.
     * @param aModuleType Type of the module that has finished.
     */
    void FinishedSignal(Types::eLoadingModule aType);

  private:
    /**
     * @brief Enum describing possible states of the loading module.
     */
    enum class eLoadingStatus
    {
        UnLoaded = 0, //!< Loading has not started.
        Loading,      //!< Loading is in progress.
        Loaded,       //!< Loading has finished.
        Size          //!< Size of this enum.
    };

    using tResourceLoadOrderType = std::vector<Types::eResource>;                      //!< Type for the order that resources have to be loaded.
    using tResourcesPerType = std::map<Types::eLoadingModule, tResourceLoadOrderType>; //!< Type that relates module types with the resource they load.

  private:
    /**
     * @brief Launches the loading process for a given resource.
     * @param The resource we want to load.
     */
    void LaunchResourceLoader(Types::eResource aResource);

    /**
     * @brief Manages the finishing of the loading of the module.
     */
    void LoadFinished();

  private:
    u8                    mResourceIndex; //!< Indicates which resource we are loading.
    Types::eLoadingModule mModuleType;    //!< Indicates type of this module.
    eLoadingStatus        mLoadingStatus; //!< Indicates the current loading status of the module.
    QStatusBar&           mStatusBar;     //!< Bar that shows messages at the bottom of the screen.

    std::unique_ptr<CResourceLoader> mCurrentResourceLoader; //!< Loader loading the current resource.
    const static tResourcesPerType   mResources;             //!< Indicates the resources (and their order) for each module type.
};

#endif // C_LOADING_MODULE_H
