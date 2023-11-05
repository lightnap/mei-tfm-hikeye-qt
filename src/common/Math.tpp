#ifndef MATH_T
#define MATH_T

/**
 * @brief Template implementations of math functions.
 */
namespace Math
{

template<typename tFieldElement>
Vector2D<tFieldElement>::Vector2D(tFieldElement aX, tFieldElement aY)
  : oX(aX)
  , oY(aY)
{
}

template<typename tFieldElement>
Vector2D<tFieldElement> operator-(const Vector2D<tFieldElement>& aFirst, const Vector2D<tFieldElement>& aSecond)
{
    return {aFirst.oX - aSecond.oX, aFirst.oY - aSecond.oY};
}
}

#endif
