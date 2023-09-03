#include "CHeightMapResourceLoader.hpp"

#include "CResourceLoaderFactory.hpp"
#include "Types.hpp"

#include <QDir>
#include <QFile>
#include <QImage>
#include <QString>

#include <iostream> // TODO: Remove this.

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

    // Create a HeightMap structure for saving things
    // Pass it to the data manager.

    std::cout << "[HeightMapResource] Loading height map" << std::endl;
    // TODO: Fill this function.
    for (int i = 0; i < 5; i++)
    {
        std::cout << "[Height] Doing work: " << i << std::endl;

        sleep(1);
        if (isInterruptionRequested())
        {
            mLoadErrorMessage = "Interrupted by user";
            return Types::eLoadResult::Interrupted;
        }
    }

    return Types::eLoadResult::Successful;
}
