#include "CResourceLoader.hpp"

#include "CDataManager.hpp"
#include "CResourceLoaderFactory.hpp"
#include "Types.hpp"

#include <QDir>
#include <QString>

#include <iostream> //TODO: Remove this.

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
    // TODO: HK-34 Assert something here. This should not be called.
    std::cout << "WARNING: Called the LoadResource of the base module" << std::endl;

    return Types::eLoadResult::Size;
}

QString CResourceLoader::GetResourceFilePath(QString& aFile) const
{
    // TODO: Redo this.
    // QString AreaFolderPath {mDataManager.GetFolderPath()};
    // return QDir::cleanPath(AreaFolderPath) + QDir::separator() + aFile;
    return "/home/thedoa1013/code/hikeyeQt/data/matagalls/" + aFile;
}
