#include "CDataManager.hpp"

#include "CHeightMap.hpp"

#include <QString>
#include <utility>

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

void CDataManager::SetHeightMap(std::unique_ptr<CHeightMap> aHeightMap)
{
    mHeightMap = std::move(aHeightMap);
}

const CHeightMap& CDataManager::GetHeightMap() const
{
    return *mHeightMap;
}
