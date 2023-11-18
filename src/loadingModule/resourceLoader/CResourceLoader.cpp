#include "CResourceLoader.hpp"

#include "common/Types.hpp"
#include "dataStructures/CDataManager.hpp"
#include "loadingModule/resourceLoaderFactory/CResourceLoaderFactory.hpp"

#include <QDir>
#include <QFile>
#include <QString>

#include <cassert>

namespace
{
    [[maybe_unused]] const bool FactoryRegistered {CConcreteResourceLoaderFactory<CResourceLoader>::Register(Types::eResource::None)};
}

CResourceLoader::CResourceLoader(Types::eResource aResource, CDataManager& aDataManager)
  : mDataManager(aDataManager)
  , mLoadErrorMessage("")
  , mResourceType(aResource)
{
}

void CResourceLoader::run()
{
    mLoadErrorMessage = "";
    const auto ReturnValue {LoadResource()};
    emit       ResourceLoadedSignal(static_cast<int>(ReturnValue), mLoadErrorMessage);
}

Types::eLoadResult CResourceLoader::LoadResource()
{
    assert(false && "Called the LoadResource of the Base class, which should not be called");
    return Types::eLoadResult::Size;
}

Types::eLoadResult CResourceLoader::OpenFile(QString& aFileName, QFile& aFile)
{
    QString ResourceFilePath {GetResourceFilePath(aFileName)};

    if (!QFile::exists(ResourceFilePath))
    {
        mLoadErrorMessage = QString("File ") + aFileName + QString(" not found.");
        return Types::eLoadResult::Error;
    }

    aFile.setFileName(ResourceFilePath);

    const bool SuccessfullyOpenedFile {aFile.open(QIODevice::ReadOnly | QIODevice::Text)};
    if (!SuccessfullyOpenedFile)
    {
        mLoadErrorMessage = QString("File ") + aFileName + QString(" could not be opened");
        return Types::eLoadResult::Error;
    }
    else
    {
        return Types::eLoadResult::Successful;
    }
}

QString CResourceLoader::GetResourceFilePath(QString& aFile) const
{
    // TODO: Unhardcode this when we have more areas.
    // QString AreaFolderPath {mDataManager.GetFolderPath()};
    // return QDir::cleanPath(AreaFolderPath) + QDir::separator() + aFile;
    return "/home/thedoa1013/code/hikeyeQt/data/matagalls/" + aFile;
}
