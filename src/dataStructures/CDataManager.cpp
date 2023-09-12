#include "CDataManager.hpp"

#include "SHeightMap.hpp"
#include "STerrain.hpp"

#include <QString>
#include <utility>

// TODO: Think about thread safety and whether data mutexes are worth it.

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
