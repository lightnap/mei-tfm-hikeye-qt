#include "CGroundTruthResourceLoader.hpp"

#include "CDataManager.hpp"
#include "CResourceLoaderFactory.hpp"
#include "Math.hpp"
#include "SGroundTruth.hpp"
#include "Types.hpp"

#include <QFile>
#include <QString>
#include <QXmlStreamReader>

#include <iostream> //TODO: HK-25 Remove this.
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
    std::cout << "[GroundTruthResource] Loading ground truth" << std::endl;

    // TODO: This could be its own function, but it should be in a try catch block so that it can return in the midle of the thing.
    QString ResourceFilePath {GetResourceFilePath(FILE_NAME)};

    if (!QFile::exists(ResourceFilePath))
    {
        mLoadErrorMessage = "GroundTruth file not found.";
        return Types::eLoadResult::Error;
    }

    QFile      File {ResourceFilePath};
    const bool SuccessfullyOpenedFile {File.open(QIODevice::ReadOnly | QIODevice::Text)};
    if (!SuccessfullyOpenedFile)
    {
        mLoadErrorMessage = "Ground truth file could not be opened";
        return Types::eLoadResult::Error;
    }

    QXmlStreamReader Xml;
    Xml.setDevice(&File);
    SGroundTruth::tNetwork GroundTruthNetwork;

    s32 WayIndex {0};

    while (Xml.readNextStartElement())
    {
        if (Xml.name() == "note" || Xml.name() == "meta")
        {
            Xml.skipCurrentElement();
        }
        else if (Xml.name() == "way")
        {
            // TODO: Move all this into parse way funcion.
            GroundTruthNetwork.emplace_back();
            while (Xml.readNextStartElement())
            {
                if (Xml.name() == "bounds")
                {
                    Xml.skipCurrentElement();
                    // TODO: Handle bounds.
                }
                else if (Xml.name() == "nd")
                {
                    f32 Northing {Xml.attributes().value("northing").toFloat()};
                    f32 Easting {Xml.attributes().value("easting").toFloat()};
                    GroundTruthNetwork.at(WayIndex).emplace_back(Northing, Easting);
                    Xml.skipCurrentElement();
                }
                else
                {
                    Xml.skipCurrentElement();
                }
            }
            WayIndex++;
            if (isInterruptionRequested())
            {
                mLoadErrorMessage = "Interrupted by user";
                return Types::eLoadResult::Interrupted;
            }
        }
    }

    std::unique_ptr<SGroundTruth> GroundTruth {std::make_unique<SGroundTruth>()};
    GroundTruth->oNetwork = std::move(GroundTruthNetwork);
    mDataManager.SetGroundTruth(std::move(GroundTruth));

    return Types::eLoadResult::Successful;
}
