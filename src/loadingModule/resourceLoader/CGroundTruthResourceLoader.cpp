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
        Network.emplace_back(std::move(utilities::CWktParser::Parse(Row.at(0))));
    }

    std::unique_ptr<SGroundTruth> GroundTruth {std::make_unique<SGroundTruth>(std::move(Network))};
    mDataManager.SetGroundTruth(std::move(GroundTruth));

    return Types::eLoadResult::Successful;
}
