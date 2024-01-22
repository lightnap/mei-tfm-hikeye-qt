#ifndef CWKTPARSER_H
#define CWKTPARSER_H

#include <string>
#include <vector>

#include "common/Math.hpp"

namespace utilities
{
    /**
     * @brief Class to parse wkt. Only supports linestrings. We assume well+formedess.
     */
    class CWktParser
    {
      public:
        using tPoint = Math::Vector2D<f64>;      //!< Type to represent a wkt point.
        using tLineString = std::vector<tPoint>; //!< Type to represet a wkt linestring.

        /**
         * @brief Parses the given wkt input and returns a vector of points.
         * @param aString: Reference to the string containing the linestring.
         * @return A vector of points.
         */
        static tLineString Parse(const std::string& aString);

      private:
        /**
         * @brief Enum descriging the possible state the parser my be in.
         */
        enum class eWktState
        {
            Header = 0,       //!< Wkt header, such as "LINESTRING".
            FirstCoordinate,  //!< Parsing the first coordinate.
            SecondCoordinate, //!< Parsing the second coordinate
            Finished,         //!< Parsing has finished.
        };
    };
}

#endif // CWKTPARSER_H
