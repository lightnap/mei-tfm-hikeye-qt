#include "CGroundTruthResourceLoader.hpp"

#include "common/Math.hpp"
#include "common/Types.hpp"
#include "dataStructures/CDataManager.hpp"
#include "dataStructures/SGroundTruth.hpp"
#include "loadingModule/resourceLoaderFactory/CResourceLoaderFactory.hpp"
#include "utilities/CCsvParser.hpp"
#include "utilities/CWktParser.hpp"

#include <QFile>
#include <QString>
#include <QTextStream>
#include <QXmlStreamReader>

#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace
{
    QString FILE_NAME {"edges.csv"};

    [[maybe_unused]] const bool FactoryRegistered {CConcreteResourceLoaderFactory<CGroundTruthResourceLoader>::Register(Types::eResource::GroundTruth)};
}

Types::eLoadResult CGroundTruthResourceLoader::LoadResource()
{
    std::cout << "[GroundTruthResource] Loading ground truth" << std::endl;

    QFile              File;
    Types::eLoadResult OpenFileResult {OpenFile(FILE_NAME, File)};
    if (OpenFileResult != Types::eLoadResult::Successful)
    {
        return OpenFileResult;
    }

    QTextStream                    Stream {&File};
    utilities::CCsvParser::tResult NetworkCSV {utilities::CCsvParser::Parse(Stream)};

    SGroundTruth::tNetwork Network;

    for (const auto& Row : NetworkCSV)
    {
        const auto& Points {utilities::CWktParser::Parse(Row.at(0))};

        SGroundTruth::STrack Track;

        for (u32 Index {0U}; Index < Points.size(); Index++)
        {
            const auto& Point {Points.at(Index)};
            s64 OsmId {std::stoll(Row.at(Index + 1U))}; // TODO: This does not work if we have more than 2 points in the track, as the csv only has info on the first and the last.
            Track.oPoints.emplace_back(SGroundTruth::SPoint {.oNorthing = Point.oY, .oEasting = Point.oX, .oOsmId = OsmId});
        }

        Network.emplace_back(std::move(Track));
    }

    std::unique_ptr<SGroundTruth> GroundTruth {std::make_unique<SGroundTruth>(std::move(Network))};
    mDataManager.SetGroundTruth(std::move(GroundTruth));

    return Types::eLoadResult::Successful;
}
