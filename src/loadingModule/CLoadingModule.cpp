#include "CLoadingModule.hpp"
#include "Types.hpp"

#include <QStatusBar>
#include <iostream>
#include <string>

// clang-format off
const CLoadingModule::tResourcesPerType CLoadingModule::mResources
{
    {Types::eLoadingModule::Terrain, {Types::eResource::HeightMap, Types::eResource::Terrain}},
    {Types::eLoadingModule::Tracks,  {Types::eResource::GroundTruth, Types::eResource::Matches, Types::eResource::Queries, Types::eResource::Texture},}
};
// clang-format on

CLoadingModule::CLoadingModule(Types::eLoadingModule aModuleType, QStatusBar& aStatusBar)
  : mResourceIndex(0U)
  , mModuleType(aModuleType)
  , mLoadingStatus(eLoadingStatus::UnLoaded)
  , mStatusBar(aStatusBar)
  , mCurrentResourceLoader(nullptr)
{
}

void CLoadingModule::LaunchLoader()
{
    mLoadingStatus = eLoadingStatus::Loading;
    mResourceIndex = 0U;

    const std::string ModuleType {Types::LoadingModuleToString(mModuleType)};
    mStatusBar.showMessage(("Loading " + ModuleType).c_str(), 2000);

    const auto FirstResource {mResources.at(mModuleType).at(mResourceIndex)};
    LaunchResourceLoader(FirstResource);
}

void CLoadingModule::LaunchResourceLoader(Types::eResource aResource)
{
    std::cout << "Loading resource" + Types::ResourceToString(aResource) << std::endl;
    ResourceFinishedLoading();
    // TODO: Create new resource loader. Using a factory.
    // mCurrentResourceLoader->start();
}

void CLoadingModule::ResourceFinishedLoading()
{
    mResourceIndex++;
    mCurrentResourceLoader.reset(nullptr);

    const auto& ResourcesOrder {mResources.at(mModuleType)};

    if (mResourceIndex < static_cast<u8>(ResourcesOrder.size()))
    {
        LaunchResourceLoader(ResourcesOrder.at(mResourceIndex));
    }
    else
    {
        LoadFinished();
    }
}

void CLoadingModule::LoadFinished()
{
    mLoadingStatus = eLoadingStatus::Loaded;
    const std::string ModuleType {Types::LoadingModuleToString(mModuleType)};
    mStatusBar.showMessage(("Finished loading " + ModuleType).c_str(), 2000);
    emit FinishedSignal(mModuleType);
}

void CLoadingModule::CancelLoad()
{
    mCurrentResourceLoader->quit();
    mLoadingStatus = eLoadingStatus::UnLoaded;
    mResourceIndex = 0U;
}

bool CLoadingModule::IsUnLoaded() const
{
    return mLoadingStatus == eLoadingStatus::UnLoaded;
}

bool CLoadingModule::IsLoading() const
{
    return mLoadingStatus == eLoadingStatus::Loading;
}

bool CLoadingModule::IsLoaded() const
{
    return mLoadingStatus == eLoadingStatus::Loaded;
}
