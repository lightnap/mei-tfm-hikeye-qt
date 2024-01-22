#include "SMatches.hpp"

#include <utility>

SMatches::SMatches(const tMatches&& aMatches)
  : oMatches(std::move(aMatches))
{
}
