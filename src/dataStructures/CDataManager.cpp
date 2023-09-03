#include "CDataManager.hpp"

#include <QString>

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
