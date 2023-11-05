#include "CDataManager.hpp"

#include "dataStructures/SGroundTruth.hpp"
#include "dataStructures/SHeightMap.hpp"
#include "dataStructures/STerrain.hpp"
#include "dataStructures/STexture.hpp"

#include <QString>
#include <utility>

// TODO: HK-44 Think about thread safety and whether data mutexes are worth it.

CDataManager::CDataManager()
  : mDataFolderPath("")
{
    mDataMutex = std::make_unique<QMutex>();
}

void CDataManager::SetFolderPath(QString aFolderPath)
{
    mDataMutex->lock();
    mDataFolderPath = aFolderPath;
    mDataMutex->unlock();
}

QString CDataManager::GetFolderPath() const
{
    return mDataFolderPath;
}

void CDataManager::SetHeightMap(std::unique_ptr<SHeightMap> aHeightMap)
{
    mHeightMap = std::move(aHeightMap);
}

const SHeightMap& CDataManager::GetHeightMap() const
{
    return *mHeightMap;
}

void CDataManager::SetTerrain(std::unique_ptr<STerrain> aTerrain)
{
    mTerrain = std::move(aTerrain);
}

const STerrain& CDataManager::GetTerrain() const
{
    return *mTerrain;
}

void CDataManager::SetGroundTruth(std::unique_ptr<SGroundTruth> aGroundTruth)
{
    mGroundTruth = std::move(aGroundTruth);
}

const SGroundTruth& CDataManager::GetGroundTruth() const
{
    return *mGroundTruth;
}

void CDataManager::SetTexture(std::unique_ptr<STexture> aTexture)
{
    mTexture = std::move(aTexture);
}

const STexture& CDataManager::GetTexture() const
{
    return *mTexture;
}
