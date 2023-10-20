#ifndef C_DATA_MANAGER_H
#define C_DATA_MANAGER_H

#include <QMutex>
#include <QString>

#include "SGroundTruth.hpp"
#include "SHeightMap.hpp"
#include "STerrain.hpp"

#include <memory>

/**
 * @brief Class that stores and manages all data from the various areas.
 */
class CDataManager
{
  public:
    /**
     * Constructor.
     */
    CDataManager();

    /**
     * @brief Sets the folder path from where we will store/load all area files.
     * @param aFolderPath: Path for the folder we want to set.
     */
    void SetFolderPath(QString aFolderPath);

    /**
     * @brief Gets the folder path in which we store/load all area files.
     * @return Path for the folder.
     */
    QString GetFolderPath() const;

    /**
     * @brief Sets the area height map. Takes ownership of the object.
     * @param aHeightMap: pointer to the height map.
     */
    void SetHeightMap(std::unique_ptr<SHeightMap> aHeightMap);

    /**
     * @brief Gets the area height map.
     * @return A reference to the area height map.
     */
    const SHeightMap& GetHeightMap() const;

    /**
     * @brief Sets the area terrain model. Takes ownership of the object.
     * @param aTerrain: pointer to the height map.
     */
    void SetTerrain(std::unique_ptr<STerrain> aTerrain);

    /**
     * @brief Gets the area terrain model.
     * @return A reference to the area terrain model.
     */
    const STerrain& GetTerrain() const;

    /**
     * @brief Sets the area ground truth. Takes ownership of the object.
     * @param aGroundTruth: pointer to the area ground truth.
     */
    void SetGroundTruth(std::unique_ptr<SGroundTruth> aGroundTruth);

    /**
     * @brief Gets the area ground truth.
     * @return The area ground truth.
     */
    const SGroundTruth& GetGroundTruth() const;

  private:
    QString                       mDataFolderPath; //!< Folder path in which we store/load all area files.
    std::unique_ptr<QMutex>       mDataMutex;      //!< Mutex for safely accessing all data.
    std::unique_ptr<SHeightMap>   mHeightMap;      //!< Height map of the area.
    std::unique_ptr<STerrain>     mTerrain;        //!< 3D model of area terrain.
    std::unique_ptr<SGroundTruth> mGroundTruth;    //!< Path network we assume is accurate.
};
#endif // C_DATA_MANAGER_H
