#include "SGroundTruth.hpp"

#include <utility>

SGroundTruth::SGroundTruth(const tNetwork&& aNetwork)
  : oNetwork(std::move(aNetwork))
{
}
