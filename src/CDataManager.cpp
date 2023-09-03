#include "CDataManager.hpp"

#include <QString>

CDataManager::CDataManager() {}

void CDataManager::SetFolderPath(QString aFolderPath)
{
    mDataFolderPath = aFolderPath;
}

QString CDataManager::GetFolderPath() const
{
    return mDataFolderPath;
}
