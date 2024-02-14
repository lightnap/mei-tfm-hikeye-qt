#include "CSaveImageResourceLoader.hpp"

#include "common/Types.hpp"
#include "dataStructures/SMatches.hpp"
#include "loadingModule/resourceLoaderFactory/CResourceLoaderFactory.hpp"

#include <QDir>
#include <QImage>
#include <QString>

#include <iostream>
#include <memory>

namespace
{
    QString SAVE_TO_FILE_NAME {"tracksTexture.png"};

    [[maybe_unused]] const bool FactoryRegistered {CConcreteResourceLoaderFactory<CSaveImageResourceLoader>::Register(Types::eResource::SaveImage)};
}

Types::eLoadResult CSaveImageResourceLoader::LoadResource()
{
    std::cout << "[ImageSaver] Saving track texture image..." << std::endl;

    QString SaveToFilePath {GetResourceFilePath(SAVE_TO_FILE_NAME)};

    const QImage& TextureImage {mDataManager.GetTexture().oTexture};

    // TODO: HK-54 Why are we mirroring this??
    TextureImage.mirrored(true, false).save(SaveToFilePath);

    return Types::eLoadResult::Successful;
}
