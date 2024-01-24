#include "SGroundTruth.hpp"

#include <utility>

#include <iostream>

SGroundTruth::SGroundTruth(const tNetwork&& aNetwork)
  : oNetwork(std::move(aNetwork))
{
}
