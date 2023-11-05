#include "CGroundTruthResourceLoader.hpp"

#include "common/Math.hpp"
#include "common/Types.hpp"
#include "dataStructures/CDataManager.hpp"
#include "dataStructures/SGroundTruth.hpp"
#include "loadingModule/resourceLoaderFactory/CResourceLoaderFactory.hpp"

#include <QFile>
#include <QString>
#include <QXmlStreamReader>

#include <iostream> // TODO: HK-25 Remove this.
#include <memory>
#include <utility>
#include <vector>

namespace
{
QString FILE_NAME {"groundTruthUtm.gpx"};

[[maybe_unused]] const bool FactoryRegistered {CConcreteResourceLoaderFactory<CGroundTruthResourceLoader>::Register(Types::eResource::GroundTruth)};
}

Types::eLoadResult CGroundTruthResourceLoader::LoadResource()
{
    // TODO: Remove this.
    std::cout << "[GroundTruthResource] Loading ground truth" << std::endl;

    QFile              File;
    Types::eLoadResult OpenFileResult {OpenFile(FILE_NAME, File)};
    if (OpenFileResult != Types::eLoadResult::Successful)
    {
        return OpenFileResult;
    }

    QXmlStreamReader Xml;
    Xml.setDevice(&File);
    SGroundTruth::tNetwork GroundTruthNetwork;

    Types::eLoadResult ParseXmlResult {FillGroundTruth(Xml, GroundTruthNetwork)};
    if (ParseXmlResult != Types::eLoadResult::Successful)
    {
        return OpenFileResult;
    }

    std::unique_ptr<SGroundTruth> GroundTruth {std::make_unique<SGroundTruth>(std::move(GroundTruthNetwork))};
    mDataManager.SetGroundTruth(std::move(GroundTruth));

    return Types::eLoadResult::Successful;
}

Types::eLoadResult CGroundTruthResourceLoader::FillGroundTruth(QXmlStreamReader& aXml, SGroundTruth::tNetwork& aNetwork)
{
    while (aXml.readNextStartElement())
    {
        if (aXml.name() == "note" || aXml.name() == "meta")
        {
            aXml.skipCurrentElement();
        }
        else if (aXml.name() == "way")
        {
            aNetwork.emplace_back(ParseWay(aXml));

            if (isInterruptionRequested())
            {
                mLoadErrorMessage = "Interrupted by user";
                return Types::eLoadResult::Interrupted;
            }
        }
    }

    return Types::eLoadResult::Successful;
}

SGroundTruth::tTrack CGroundTruthResourceLoader::ParseWay(QXmlStreamReader& aXml)
{
    SGroundTruth::tTrack Track {};

    while (aXml.readNextStartElement())
    {
        if (aXml.name() == "nd")
        {
            f32 Northing {aXml.attributes().value("northing").toFloat()};
            f32 Easting {aXml.attributes().value("easting").toFloat()};

            // TODO: Do not hardcode this.
            Math::Vector2D<f64> Min(444825.0, 4633335.0 - 4017.0 * 2.0);
            Math::Vector2D<f64> Max(444825.0 + 3725.0 * 2.0, 4633335.0);

            if (Min.oX < Easting && Easting < Max.oX && Min.oY < Northing && Northing < Max.oY)
            {
                Track.emplace_back(Easting, Northing);
            }
            aXml.skipCurrentElement();
        }
        else
        {
            aXml.skipCurrentElement();
        }
    }

    return Track;
}
