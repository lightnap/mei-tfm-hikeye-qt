#include "CDataManager.hpp"

#include "dataStructures/SGroundTruth.hpp"
#include "dataStructures/SHeightMap.hpp"
#include "dataStructures/STerrain.hpp"
#include "dataStructures/STexture.hpp"

#include <QString>
#include <utility>

// TODO: HK-44 Think about thread safety and whether data mutexes are worth it.

CDataManager::CDataManager()
  : mDataFolderPath("")
{
    mDataMutex = std::make_unique<QMutex>();
}

void CDataManager::SetPaintStrategy(Types::ePaintStrategy aStrategy)
{
    mPaintStrategy = aStrategy;
}

Types::ePaintStrategy CDataManager::GetPaintStrategy() const
{
    return mPaintStrategy;
}

void CDataManager::SetDateFilter(Types::sDateFilter aFilter)
{
    mDateFilter = aFilter;
}

Types::sDateFilter CDataManager::GetDateFilter() const
{
    return mDateFilter;
}

void CDataManager::SetFolderPath(QString aFolderPath)
{
    mDataMutex->lock();
    mDataFolderPath = aFolderPath;
    mDataMutex->unlock();
}

QString CDataManager::GetFolderPath() const
{
    return mDataFolderPath;
}

void CDataManager::SetHeightMap(std::unique_ptr<SHeightMap> aHeightMap)
{
    mHeightMap = std::move(aHeightMap);
}

const SHeightMap& CDataManager::GetHeightMap() const
{
    return *mHeightMap;
}

void CDataManager::SetTerrain(std::unique_ptr<STerrain> aTerrain)
{
    mTerrain = std::move(aTerrain);
}

const STerrain& CDataManager::GetTerrain() const
{
    return *mTerrain;
}

void CDataManager::SetGroundTruth(std::unique_ptr<SGroundTruth> aGroundTruth)
{
    mGroundTruth = std::move(aGroundTruth);
}

const SGroundTruth& CDataManager::GetGroundTruth() const
{
    return *mGroundTruth;
}

void CDataManager::SetMatches(std::unique_ptr<SMatches> aMatches)
{
    mMatches = std::move(aMatches);
}

const SMatches& CDataManager::GetMatches() const
{
    return *mMatches;
}

void CDataManager::SetQueries(std::unique_ptr<SQueries> aQueries)
{
    mQueries = std::move(aQueries);
}

const SQueries& CDataManager::GetQueries() const
{
    return *mQueries;
}

void CDataManager::SetTerrainTexture(std::unique_ptr<STexture> aTexture)
{
    mTerrainTexture = std::move(aTexture);
}

const STexture& CDataManager::GetTerrainTexture() const
{
    return *mTerrainTexture;
}

void CDataManager::SetTracksTexture(std::unique_ptr<STexture> aTexture)
{
    mTracksTexture = std::move(aTexture);
}

const STexture& CDataManager::GetTracksTexture() const
{
    return *mTracksTexture;
}
