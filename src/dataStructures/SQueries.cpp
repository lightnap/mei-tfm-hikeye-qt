#include "SQueries.hpp"

#include <utility>

SQueries::SQueries(const tCrossingsInfo&& aCrossingsInfo)
  : oCrossingsInfo(std::move(aCrossingsInfo))
{
}
