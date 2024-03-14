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
    Vector2D<double> Vector2D<tFieldElement>::Normalize()
    {
        const f64 Norm {std::sqrt(oX * oX + oY * oY)};
        return {oX / Norm, oY / Norm};
    }

    template<typename tFieldElement>
    Vector2D<tFieldElement> operator-(const Vector2D<tFieldElement>& aFirst, const Vector2D<tFieldElement>& aSecond)
    {
        return {aFirst.oX - aSecond.oX, aFirst.oY - aSecond.oY};
    }

    template<typename tFieldElement>
    Vector2D<tFieldElement> operator+(const Vector2D<tFieldElement>& aFirst, const Vector2D<tFieldElement>& aSecond)
    {
        return {aFirst.oX + aSecond.oX, aFirst.oY + aSecond.oY};
    }

    template<typename tFieldElement>
    Vector2D<tFieldElement> operator*(f64 aScalar, const Vector2D<tFieldElement>& aVector)
    {
        return {aScalar * aVector.oX, aScalar * aVector.oY};
    }

}

#endif
