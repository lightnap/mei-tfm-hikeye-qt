#include "CWktParser.hpp"

namespace utilities
{
    CWktParser::tLineString CWktParser::Parse(const std::string& aString)
    {
        eWktState   State {eWktState::Header};
        tLineString Result {};
        size_t      PointIndex {0};

        std::string FirstCoordinateStr {};
        std::string SecondCoordinateStr {};

        f64 FirstCoordinate {0.0};
        f64 SecondCoordinate {0.0};

        for (const char CurrentChar : aString)
        {
            switch (State)
            {
                case eWktState::Header:
                    switch (CurrentChar)
                    {
                        case '(': // end of header
                            State = eWktState::FirstCoordinate;
                            break;
                        default:
                            break;
                    }
                    break;
                case eWktState::FirstCoordinate:
                    switch (CurrentChar)
                    {
                        case ' ':
                            State = eWktState::SecondCoordinate;
                            break;
                        default:
                            FirstCoordinateStr.push_back(CurrentChar);
                            break;
                    }
                    break;
                case eWktState::SecondCoordinate:
                    switch (CurrentChar)
                    {
                        case ',':
                            FirstCoordinate = std::stod(FirstCoordinateStr);
                            SecondCoordinate = std::stod(SecondCoordinateStr);
                            Result.emplace_back(Math::Vector2D<f64> {FirstCoordinate, SecondCoordinate});

                            FirstCoordinateStr = "";
                            SecondCoordinateStr = "";
                            State = eWktState::FirstCoordinate;
                            break;
                        case ')':
                            FirstCoordinate = std::stod(FirstCoordinateStr);
                            SecondCoordinate = std::stod(SecondCoordinateStr);
                            Result.emplace_back(Math::Vector2D<f64> {FirstCoordinate, SecondCoordinate});

                            FirstCoordinateStr = "";
                            SecondCoordinateStr = "";
                            State = eWktState::Finished;
                            break;
                        default:
                            // Save char
                            SecondCoordinateStr.push_back(CurrentChar);
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
        return Result;
    }
}
