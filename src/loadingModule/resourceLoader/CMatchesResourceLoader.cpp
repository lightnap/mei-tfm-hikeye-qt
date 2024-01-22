#include "CMatchesResourceLoader.hpp"

#include "common/Types.hpp"
#include "dataStructures/SMatches.hpp"
#include "loadingModule/resourceLoaderFactory/CResourceLoaderFactory.hpp"

#include <QDir>
#include <QString>
#include <QTextStream>

#include <iostream>
#include <memory>

namespace
{
    [[maybe_unused]] const bool FactoryRegistered {CConcreteResourceLoaderFactory<CMatchesResourceLoader>::Register(Types::eResource::Matches)};
}

Types::eLoadResult CMatchesResourceLoader::LoadResource()
{
    std::cout << "[MatchesResource] Loading matches" << std::endl;
    // TODO: HK-24 Fill this function.
    QDir Directory {"/home/lightnap/src/hikeyeQt/data/matagalls/matches"};

    Directory.setFilter(QDir::Files);

    SMatches::tMatches Matches;

    for (const auto& FileName : Directory.entryList())
    {
        QFile              File {FileName};
        QString            FilePath {QString("matches/") + FileName};
        Types::eLoadResult OpenFileResult {OpenFile(FilePath, File)};

        if (OpenFileResult != Types::eLoadResult::Successful)
        {
            return OpenFileResult;
        }

        QTextStream Stream {&File};
        QString     CurrentLine;

        while (Stream.readLineInto(&CurrentLine))
        {
            Matches.emplace_back(ParseLine(CurrentLine.toStdString()));
        }
    }

    std::unique_ptr<SMatches> MatchesStruct {std::make_unique<SMatches>(std::move(Matches))};
    mDataManager.SetMatches(std::move(MatchesStruct));

    return Types::eLoadResult::Successful;
}

std::vector<u32> CMatchesResourceLoader::ParseLine(const std::string& aCurrentLine)
{
    std::string      CurrentNumber {};
    std::vector<u32> ParsedNumbers {};

    for (const char CurrentChar : aCurrentLine)
    {
        if (CurrentChar == ',')
        {
            ParsedNumbers.push_back(std::stoi(CurrentNumber));
            CurrentNumber = "";
        }
        else
        {
            CurrentNumber.push_back(CurrentChar);
        }
    }
    return ParsedNumbers;
}
