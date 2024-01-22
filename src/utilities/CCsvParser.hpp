#ifndef CCSVPARSER_H
#define CCSVPARSER_H

#include <string>
#include <vector>

#include <QFile>
#include <QTextStream>

namespace utilities
{
    /**
     * @brief Class to parse csv files.
     */
    class CCsvParser
    {
      public:
        using tRow = std::vector<std::string>; //!< Type representing a row of the csv.
        using tResult = std::vector<tRow>;     //!< Type representing the resulf of the parsing prcess.

        /**
         * @brief Parses the given csv file.
         * @param aStream: A referece text stream initialized with the csv file,
         * @return The parsed csv file, as a vector of row; each row a vector of fields.
         */
        static tResult Parse(QTextStream& aStream);

      private:
        /**
         * @brief Enum describing the possible states the parser may reach when parsing a field.
         */
        enum class eCSVState
        {
            UnquotedField = 0, //!< Outise quotes.
            QuotedField,       //!< Inside quotes.
            QuotedQuote,       //!< We were inside quotes, have exited quotes, but have not exited the field yet.
        };

        /**
         * @brief Parses a row of the csv. Turns a string into tokens. Assuming comma is separator and quotes is quotes.
         * @param aRow A string representing the csv row.
         * @return A vector of the separated tokens.
         */
        static tRow ParseRow(const std::string& aRow);
    };
}

#endif // CCSVPARSER_H
