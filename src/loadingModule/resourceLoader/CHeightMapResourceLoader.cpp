#include "CHeightMapResourceLoader.hpp"

#include "CResourceLoaderFactory.hpp"
#include "SHeightMap.hpp"
#include "Types.hpp"

#include <QDir>
#include <QFile>
#include <QImage>
#include <QString>

#include <iostream> // TODO: Remove this.
#include <memory>
#include <utility>

namespace
{
QString FILE_NAME {"dem.png"};

[[maybe_unused]] const bool FactoryRegistered {CConcreteResourceLoaderFactory<CHeightMapResourceLoader>::Register(Types::eResource::HeightMap)};
}

Types::eLoadResult CHeightMapResourceLoader::LoadResource()
{
    QString ResourceFilePath {GetResourceFilePath(FILE_NAME)};
    std::cout << "[Height] LoadingFile: " << ResourceFilePath.toStdString() << std::endl;

    if (!QFile::exists(ResourceFilePath))
    {
        mLoadErrorMessage = "Height map file not found.";
        return Types::eLoadResult::Error;
    }

    QImage HeightMapTexture;
    HeightMapTexture.load(ResourceFilePath);
    auto HeightMap {std::make_unique<SHeightMap>(HeightMapTexture)};
    mDataManager.SetHeightMap(std::move(HeightMap));

    if (isInterruptionRequested())
    {
        mLoadErrorMessage = "Interrupted by user";
        return Types::eLoadResult::Interrupted;
    }

    return Types::eLoadResult::Successful;
}
