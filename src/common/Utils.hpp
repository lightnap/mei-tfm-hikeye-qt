#ifndef UTILS_H
#define UTILS_H

#include "Types.hpp"

/**
 * @brief Utilities functions to be used globally.
 */
namespace Utils
{
/**
 * @brief Performs a linear interpolation on double values.
 * @param aInitialValue: a.
 * @param aFinalValue: b.
 * @param aLerpParam: t.
 * @retun b*t + (1-t)*a.
 */
inline f64 Lerp(f64 aInitialValue, f64 aFinalValue, f64 aLerpIndex)
{
    return aInitialValue + aLerpIndex * (aFinalValue - aInitialValue);
}
}

#endif // UTILS_H
