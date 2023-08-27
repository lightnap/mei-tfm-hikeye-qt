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
     * @brief Asks current loading proces to stop (if there is one).
     */
    void LaunchCancelLoad();

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
     * @brief Returns whether we are currenty canceling the load of this module.
     * @return True if we are caneling, false otherwise.
     */
    [[nodiscard]] bool IsCanceling() const;

    /**
     * @brief Returns whether this module is fully loaded.
     * @return True if module finished loading, false otherwise.
     */
    [[nodiscard]] bool IsLoaded() const;

  public slots:
    /**
     * @brief Update info about the exit error code of the current resource loader.
     * @param aErrorCode: error code we want to notify.
     */
    void UpdateResourceLoaderErrorCode(Types::eResourceLoadingError aErrorCode);

    /**
     * @brief Gets called when a given resource loader finished.
     */
    void ResourceLoaderFinished();

  signals:
    /**
     * @brief Signal to signalize that the module has finished loading.
     * @param aModuleType Type of the module that has finished.
     */
    void FinishedSignal(Types::eLoadingModule aType);

    /**
     * @brief Signal to notify loading module cancel finished.
     */
    void LoadCanceled();

    /**
     * @brief Signal to notify the resource loaders they should load a resource.
     * @param aResource: resource loader we want to launch.
     */
    void LaunchResourceLoaderSignal(Types::eResource aResource);

  private:
    /**
     * @brief Enum describing possible states of the loading module.
     */
    enum class eLoadingStatus
    {
        UnLoaded = 0, //!< Loading has not started.
        Loading,      //!< Loading is in progress.
        Canceling,    //!< We are cancelling the loading.
        Loaded,       //!< Loading has finished.
        Size          //!< Size of this enum.
    };

    using tResourcesMap = std::map<Types::eResource, std::unique_ptr<CResourceLoader>>; //!< Type that relates a resource type to its loader.

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
    u8                           mResourceIndex;         //!< Indicates which resource we are loading.
    Types::eLoadingModule        mModuleType;            //!< Indicates type of this module.
    Types::eResourceLoadingError mResourceLoadErrorCode; //!< Error code returned by the ResourceLoader thread.
    eLoadingStatus               mLoadingStatus;         //!< Indicates the current loading status of the module.
    QStatusBar&                  mStatusBar;             //!< Bar that shows messages at the bottom of the screen.

    tResourcesMap mResourceLoaders; //!< Map of all the resource loaders of this module.
};

#endif // C_LOADING_MODULE_H
