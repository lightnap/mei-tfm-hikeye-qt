#include "CLoadingModule.hpp"

#include <QStatusBar>
#include <string>

const CLoadingModule::tResourcesPerType CLoadingModule::mResources {{eModuleType::Terrain, {eResource::HeightMap, eResource::Terrain}},
                                                                    {eModuleType::Tracks, {eResource::GroundTruth, eResource::Matches, eResource::Queries, eResource::Texture}}};

CLoadingModule::CLoadingModule(CLoadingModule::eModuleType aModuleType, QStatusBar& aStatusBar)
  : mResourceIndex(0U)
  , mModuleType(aModuleType)
  , mLoadingStatus(eLoadingStatus::UnLoaded)
  , mStatusBar(aStatusBar)
{
}

void CLoadingModule::LaunchLoader()
{
    mLoadingStatus = eLoadingStatus::Loading;
    mResourceIndex = 0U;

    const std::string ModuleType {ModuleTypeToString(mModuleType)};
    mStatusBar.showMessage(("Loading " + ModuleType).c_str(), 2000);
}

void CLoadingModule::LaunchResourceLoader(eResource aResource)
{
    // TODO: Launch resource Loaders.
}

void CLoadingModule::ResourceFinishedLoading()
{
    mResourceIndex++;
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
    const std::string ModuleType {ModuleTypeToString(mModuleType)};
    mStatusBar.showMessage(("Finished loading " + ModuleType).c_str(), 2000);
    emit FinishedSignal(mModuleType);
}

void CLoadingModule::CancelLoad()
{
    // TODO: Call ResourceLoaders[mResourceIndex].CancelLoad
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

std::string CLoadingModule::ModuleTypeToString(CLoadingModule::eModuleType aModuleType)
{
    switch (aModuleType)
    {
        case eModuleType::Terrain:
        {
            return "Terrain";
            break;
        }
        case eModuleType::Tracks:
        {
            return "Tracks";
            break;
        }
        default:
        {
            break;
        }
    }
    // TODO: Add an assert here.
    return "";
}

std::string CLoadingModule::ResourceToString(CLoadingModule::eResource aResource)
{
    switch (aResource)
    {
        case eResource::HeightMap:
        {
            return "HeightMap";
            break;
        }
        case eResource::Terrain:
        {
            return "Terrain";
            break;
        }
        case eResource::GroundTruth:
        {
            return "GroundTruth";
            break;
        }
        case eResource::Matches:
        {
            return "Matches";
            break;
        }
        case eResource::Queries:
        {
            return "Queries";
            break;
        }
        case eResource::Texture:
        {
            return "Texture";
            break;
        }
        default:
        {
            break;
        }
    }
    // TODO: Add an assertion here.
    return "";
}
