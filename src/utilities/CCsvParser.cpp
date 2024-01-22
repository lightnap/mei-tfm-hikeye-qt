#include "CCsvParser.hpp"

#include <QString>

#include <utility>

namespace utilities
{
    CCsvParser::tResult CCsvParser::Parse(QTextStream& aStream)
    {
        tResult ParseResult;

        // Discard first line (of headers.)
        QString CurrentLine {aStream.readLine()};

        while (aStream.readLineInto(&CurrentLine))
        {
            tRow Row {ParseRow(CurrentLine.toStdString())};
            ParseResult.emplace_back(std::move(Row));
        }

        return ParseResult;
    }

    CCsvParser::tRow CCsvParser::ParseRow(const std::string& aRow)
    {
        eCSVState State {eCSVState::UnquotedField};
        tRow      Fields {""};
        size_t    FieldIndex {0}; // index of the current field

        for (const char CurrentChar : aRow)
        {
            switch (State)
            {
                case eCSVState::UnquotedField:
                    switch (CurrentChar)
                    {
                        case ',': // end of field
                            Fields.push_back("");
                            FieldIndex++;
                            break;
                        case '"':
                            State = eCSVState::QuotedField;
                            break;
                        default:
                            Fields[FieldIndex].push_back(CurrentChar);
                            break;
                    }
                    break;
                case eCSVState::QuotedField:
                    switch (CurrentChar)
                    {
                        case '"':
                            State = eCSVState::QuotedQuote;
                            break;
                        default:
                            Fields[FieldIndex].push_back(CurrentChar);
                            break;
                    }
                    break;
                case eCSVState::QuotedQuote:
                    switch (CurrentChar)
                    {
                        case ',': // , after closing quote
                            Fields.push_back("");
                            FieldIndex++;
                            State = eCSVState::UnquotedField;
                            break;
                        case '"': // "" -> "
                            Fields[FieldIndex].push_back('"');
                            State = eCSVState::QuotedField;
                            break;
                        default: // end of quote
                            State = eCSVState::UnquotedField;
                            break;
                    }
                    break;
            }
        }
        return Fields;
    }
}
