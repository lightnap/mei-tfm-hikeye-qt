#include "SQueries.hpp"

#include <utility>

SQueries::SQueries(const tCrossingCount&& aCrossingCount)
  : oCrossingCount(std::move(aCrossingCount))
{
}
