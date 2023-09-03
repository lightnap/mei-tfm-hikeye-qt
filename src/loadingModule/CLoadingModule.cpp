#include "CLoadingModule.hpp"

#include "CDataManager.hpp"
#include "CResourceLoader.hpp"
#include "CResourceLoaderFactory.hpp"
#include "Types.hpp"

#include <QPointer>
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
    {Types::eLoadingModule::Tracks,  {Types::eResource::None, Types::eResource::GroundTruth, Types::eResource::Matches, Types::eResource::Queries, Types::eResource::Texture}}
}; //!< Indicates the resources (and their order) for each module type.
// clang-format on
}

CLoadingModule::CLoadingModule(Types::eLoadingModule aModuleType, CDataManager& aDataManager, QStatusBar& aStatusBar)
  : mResourceIndex(0U)
  , mModuleType(aModuleType)
  , mLoadingStatus(eLoadingStatus::UnLoaded)
  , mDataManager(aDataManager)
  , mStatusBar(aStatusBar)
  , mResourceLoaders()
{
    const auto& ResourcesToLoad {MODULE_RESOURCES.at(mModuleType)};

    for (const auto& Resource : ResourcesToLoad)
    {
        mResourceLoaders[Resource] = nullptr;
    }
}

void CLoadingModule::LaunchLoadingModule()
{
    mLoadingStatus = eLoadingStatus::Loading;
    mResourceIndex = 1U;

    const std::string ModuleType {Types::LoadingModuleToString(mModuleType)};
    mStatusBar.showMessage(("Loading " + ModuleType).c_str());

    const auto FirstResource {MODULE_RESOURCES.at(mModuleType).at(mResourceIndex)};
    LaunchResourceLoader(FirstResource);
}

void CLoadingModule::LaunchResourceLoader(Types::eResource aResource)
{
    mResourceLoaders.at(aResource) = CResourceLoaderFactory::Create(aResource, mDataManager);

    connect(mResourceLoaders.at(aResource), &CResourceLoader::ResourceLoadedSignal, this, &CLoadingModule::ResourceLoaderFinished, Qt::QueuedConnection);
    connect(mResourceLoaders.at(aResource), &CResourceLoader::finished, mResourceLoaders.at(aResource), &CResourceLoader::deleteLater, Qt::QueuedConnection);

    mResourceLoaders.at(aResource)->start();
}

void CLoadingModule::ResourceLoaderFinished(int aErrorCode)
{
    const auto ErrorCode = static_cast<Types::eResourceLoadingError>(aErrorCode);
    const auto ResourcesOrder {MODULE_RESOURCES.at(mModuleType)};

    switch (ErrorCode)
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
            mResourceIndex = 0U;
            mLoadingStatus = eLoadingStatus::UnLoaded;
            emit LoadCanceled();
        }
        default:
        {
            break;
        }
    }
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
    mLoadingStatus = eLoadingStatus::Canceling;
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
