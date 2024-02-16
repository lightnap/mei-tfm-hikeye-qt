#ifndef C_DATA_MANAGER_H
#define C_DATA_MANAGER_H

#include <QMutex>
#include <QString>

#include "common/Types.hpp"
#include "dataStructures/SGroundTruth.hpp"
#include "dataStructures/SHeightMap.hpp"
#include "dataStructures/SMatches.hpp"
#include "dataStructures/SQueries.hpp"
#include "dataStructures/STerrain.hpp"
#include "dataStructures/STexture.hpp"

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
     * @brief Sets the strategy to turn track matches data into colours.
     * @param aStrategy: The strategy to use.
     */
    void SetPaintStrategy(Types::ePaintStrategy aStrategy);

    /**
     * @brief Gets the strategy to turn track matches data into colours.
     * @return The strategy to use.
     */
    Types::ePaintStrategy GetPaintStrategy() const;

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

    /**
     * @brief Sets the matches. Takes ownership of the object.
     * @param aMatches: pointer to the matches.
     */
    void SetMatches(std::unique_ptr<SMatches> aMatches);

    /**
     * @brief Gets the matches.
     * @return The matches.
     */
    const SMatches& GetMatches() const;

    /**
     * @brief Sets the queries. Takes ownership of the object.
     * @param aQueries: pointer to the queries.
     */
    void SetQueries(std::unique_ptr<SQueries> aQueries);

    /**
     * @brief Gets the queries..
     * @return The queries.
     */
    const SQueries& GetQueries() const;

    /**
     * @brief Sets the terrain texture. Takes ownership of the object.
     * @param aTexture: pointer to the terrain texture.
     */
    void SetTexture(std::unique_ptr<STexture> aTexture);

    /**
     * @brief Gets the terrain texture.
     * @return The terrain texture.
     */
    const STexture& GetTexture() const;

  private:
    QString                       mDataFolderPath; //!< Folder path in which we store/load all area files.
    std::unique_ptr<QMutex>       mDataMutex;      //!< Mutex for safely accessing all data.
    std::unique_ptr<SHeightMap>   mHeightMap;      //!< Height map of the area.
    std::unique_ptr<STerrain>     mTerrain;        //!< 3D model of area terrain.
    std::unique_ptr<SGroundTruth> mGroundTruth;    //!< Path network we assume is accurate.
    std::unique_ptr<SMatches>     mMatches;        //!< Measured tracks matched onto the nework.
    std::unique_ptr<SQueries>     mQueries;        //!< Enriched information on the network.
    std::unique_ptr<STexture>     mTexture;        //!< Texture to draw onto the terrain.

    Types::ePaintStrategy mPaintStrategy {Types::ePaintStrategy::None}; //!< Paint strategy to paint tracks.
};
#endif // C_DATA_MANAGER_H
