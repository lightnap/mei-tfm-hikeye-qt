#include "CLoadingModule.hpp"
#include "CResourceLoaderFactory.hpp"
#include "Types.hpp"

#include <QStatusBar>
#include <iostream>
#include <memory>
#include <string>

namespace
{
using tResourceLoadOrderType = std::vector<Types::eResource>;                      //!< Type for the order that resources have to be loaded.
using tResourcesPerType = std::map<Types::eLoadingModule, tResourceLoadOrderType>; //!< Type that relates module types with the resource they load.

// clang-format off
const tResourcesPerType MODULE_RESOURCES
{
    {Types::eLoadingModule::Terrain, {Types::eResource::None, Types::eResource::HeightMap, Types::eResource::Terrain}},
    {Types::eLoadingModule::Tracks,  {Types::eResource::None, Types::eResource::GroundTruth, Types::eResource::Matches, Types::eResource::Queries, Types::eResource::Texture},}
}; //!< Indicates the resources (and their order) for each module type.
// clang-format on
}

CLoadingModule::CLoadingModule(Types::eLoadingModule aModuleType, QStatusBar& aStatusBar)
  : mResourceIndex(0U)
  , mModuleType(aModuleType)
  , mResourceLoadErrorCode(Types::eResourceLoadingError::Size)
  , mLoadingStatus(eLoadingStatus::UnLoaded)
  , mStatusBar(aStatusBar)
  , mResourceLoaders()
{
    for (const auto Resource : MODULE_RESOURCES.at(mModuleType))
    {
        auto Result {mResourceLoaders.emplace(Resource, std::unique_ptr<CResourceLoader>(CResourceLoaderFactory::Create(Resource)))};

        if (!Result.second)
        {
            // TODO: Make an assert which says we are emplacing the smae resource twice, and that is not supported.
        }
    }

    for (const auto& ResourceLoaderIt : mResourceLoaders)
    {
        auto& ResourceLoader {ResourceLoaderIt.second};
        connect(this, &CLoadingModule::LaunchResourceLoaderSignal, ResourceLoader.get(), &CResourceLoader::LoadResourceSlot);
        connect(ResourceLoader.get(), &CResourceLoader::finished, this, &CLoadingModule::ResourceLoaderFinished);
        connect(ResourceLoader.get(), &CResourceLoader::SendErrorCode, this, &CLoadingModule::UpdateResourceLoaderErrorCode);

        ResourceLoader->start();
    }
}

void CLoadingModule::LaunchLoader()
{
    mLoadingStatus = eLoadingStatus::Loading;
    mResourceIndex = 1U;

    const std::string ModuleType {Types::LoadingModuleToString(mModuleType)};
    mStatusBar.showMessage(("Loading " + ModuleType).c_str(), 2000);

    const auto FirstResource {MODULE_RESOURCES.at(mModuleType).at(mResourceIndex)};
    LaunchResourceLoader(FirstResource);
}

void CLoadingModule::LaunchResourceLoader(Types::eResource aResource)
{
    std::cout << "Loading resource" + Types::ResourceToString(aResource) << std::endl;
    emit LaunchResourceLoaderSignal(aResource);
}

void CLoadingModule::UpdateResourceLoaderErrorCode(int aErrorCode)
{
    mResourceLoadErrorCode = static_cast<Types::eResourceLoadingError>(aErrorCode);
}

void CLoadingModule::ResourceLoaderFinished()
{
    const auto ResourcesOrder {MODULE_RESOURCES.at(mModuleType)};
    const auto FinishingResource {ResourcesOrder.at(mResourceIndex)};
    mResourceLoaders.at(FinishingResource)->start();

    switch (mResourceLoadErrorCode)
    {
        case Types::eResourceLoadingError::Successful:
        {
            mResourceIndex++;
            const u8 ResourceNumber {static_cast<u8>(ResourcesOrder.size())};
            if (mResourceIndex == ResourceNumber)
            {
                LoadFinished();
            }
            else if (mResourceIndex > 0U)
            {
                LaunchResourceLoader(ResourcesOrder.at(mResourceIndex));
            }
            break;
        }
        case Types::eResourceLoadingError::UserInterruption:
        {
            mLoadingStatus = eLoadingStatus::UnLoaded;
            emit LoadCanceled();
        }
        default:
        {
            break;
        }
    }

    mResourceLoadErrorCode = Types::eResourceLoadingError::Size;
}

void CLoadingModule::LoadFinished()
{
    mLoadingStatus = eLoadingStatus::Loaded;
    const std::string ModuleType {Types::LoadingModuleToString(mModuleType)};
    mStatusBar.showMessage(("Finished loading " + ModuleType).c_str(), 2000);
    emit FinishedSignal(mModuleType);
}

void CLoadingModule::LaunchCancelLoad()
{
    const auto CurrentResource {MODULE_RESOURCES.at(mModuleType).at(mResourceIndex)};
    mResourceLoaders.at(CurrentResource)->requestInterruption();

    const auto UserInterruptionErrorCode {Types::eResourceLoadingError::UserInterruption};
    mResourceLoaders.at(CurrentResource)->exit(static_cast<u32>(UserInterruptionErrorCode));

    mLoadingStatus = eLoadingStatus::Canceling;
    mResourceIndex = 0U;
}

bool CLoadingModule::IsUnLoaded() const
{
    return mLoadingStatus == eLoadingStatus::UnLoaded;
}

bool CLoadingModule::IsCanceling() const
{
    return mLoadingStatus == eLoadingStatus::Canceling;
}

bool CLoadingModule::IsLoading() const
{
    return mLoadingStatus == eLoadingStatus::Loading;
}

bool CLoadingModule::IsLoaded() const
{
    return mLoadingStatus == eLoadingStatus::Loaded;
}
