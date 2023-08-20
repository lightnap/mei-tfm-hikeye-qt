#ifndef C_LOADING_MODULE_H
#define C_LOADING_MODULE_H

#include "Types.hpp"

#include <QObject>
#include <QStatusBar>
#include <map>
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
     * @brief Enum describing possible Loading Module types.
     */
    enum class eModuleType
    {
        Terrain = 0, //!< Loading module that loads terrain info.
        Tracks,      //!< Loading module that loads tack info.
        Size         //!< Size of this enum.
    };

  public:
    /**
     * @brief Constructor.
     * @param aModuleType Type of the module we want to contruct.
     * @param aStatusBar Reference to CMainWindow's StatusBar.
     */
    CLoadingModule(CLoadingModule::eModuleType aModuleType, QStatusBar& aStatusBar);

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
    void FinishedSignal(eModuleType aType);

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

    /**
     * @brief Enum describing the several resources we load/create during the loading process.
     */
    enum class eResource
    {
        HeightMap = 0, //!< Image file with terrain height data.
        Terrain,       //!< Terrain 3d model.
        GroundTruth,   //!< Graph of tracks we consider as the ground truth.
        Matches,       //!< Recorded tracks we want to analyze.
        Queries,       //!< Ground Truth enriched with info from tracks.
        Texture,       //!< Image containing the queries info, to be displyed on map.
        Size           //!< Size of this enum.
    };

    using tResourceLoadOrderType = std::vector<eResource>;                   //!< Type for the order that resources have to be loaded.
    using tResourcesPerType = std::map<eModuleType, tResourceLoadOrderType>; //!< Type that relates module types with the resource they load.

  private:
    /**
     * @brief Launches the loading process for a given resource.
     * @param The resource we want to load.
     */
    void LaunchResourceLoader(eResource aResource);

    /**
     * @brief Manages the finishing of the loading of the module.
     */
    void LoadFinished();

    /**
     * @brief Reflection for the eModuleType Enum.
     * @param aModuleType eModuletype enum we want to turn into string.
     * @retrun String version of the Moduletype enum.
     */
    static std::string ModuleTypeToString(eModuleType aModuleType);

    /**
     * @brief Reflection for the eResource Enum.
     * @param aModuleType Moduletype enum we want to turn into string.
     * @retrun String version of the Moduletype enum.
     */
    static std::string ResourceToString(eResource aResource);

  private:
    u8             mResourceIndex; //!< Indicates which resource we are loading.
    eModuleType    mModuleType;    //!< Indicates type of this module.
    eLoadingStatus mLoadingStatus; //!< Indicates the current loading status of the module.
    QStatusBar&    mStatusBar;     //!< Bar that shows messages at the bottom of the screen.

    const static tResourcesPerType mResources; //!< Indicates the resources (and their order) for each module type.
};

#endif // C_LOADING_MODULE_H
