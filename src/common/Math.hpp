#ifndef MATH_H
#define MATH_H

#include "common/Types.hpp"
#include <QColor>
#include <cmath>

/**
 * @brief Math functions.
 */
namespace Math
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

    /**
     * @brief Maps the interval aLowerValue, aHigherValue to [0,1]. Sends aMiddleValue to its corresponding image.
     * @param aMiddleValue: Parameter we want to map to [0,1]
     * @param aLowerValue: Lower value in the domain interval.
     * @param aHigherValue: Higher value in the domain interval.
     * @return aMiddleValue mapped to [0,1] through the mapping.
     */
    inline f64 LinearStep(f64 aMiddleValue, f64 aLowerValue, f64 aHigherValue)
    {
        if (aMiddleValue < aLowerValue)
        {
            return 0;
        }

        if (aMiddleValue > aHigherValue)
        {
            return 1;
        }

        return (aMiddleValue - aLowerValue) / (aHigherValue - aLowerValue);
    }

    /**
     * @brief Gives the value in interval [aMinValue,aMaxValue] closest to given value aValue.
     * @param aValue: Value we want to clamp.
     * @param aMinValue: Minimal value of interval.
     * @param aMaxValue: Maximal value of interval.
     * @return Clamped value.
     */
    inline f64 Clamp(f64 aValue, f64 aMinValue = 0, f64 aMaxValue = 1)
    {
        if (aValue < aMinValue)
        {
            return aMinValue;
        }
        else if (aValue < aMaxValue)
        {
            return aValue;
        }
        else
        {
            return aMaxValue;
        }
    }

    /**
     * @brief Constant depicting pi.
     */
    constexpr f64 PI = 3.14159265358979323846;

    /**
     * @brief Converts degrees into radians.
     * @param aDegree: The angle in degrees.
     * @return The angle in radians.
     */
    inline f64 DegToRad(f64 aDegree)
    {
        return (PI / 180.0) * aDegree;
    }

    /**
     * @brief Converts radians into degrees.
     * @param aRadian: The angle in radians.
     * @return The angle in degrees.
     */
    inline f64 RadToDeg(f64 aRadian)
    {
        return (180.0 / PI) * aRadian;
    }

    /**
     * @brief Struct defining a two dimensional vector of f64.
     */
    template<typename tFieldElement>
    struct Vector2D
    {
        /**
         * @brief Default constructor.
         */
        Vector2D() = default;

        /**
         * @brief Constructor.
         * @param aX: X dimension of the vector.
         * @param aY: Y dimesnion of the vector.
         */
        Vector2D(tFieldElement aX, tFieldElement aY);

        tFieldElement oX {}; //!< X coordinate of the vector.
        tFieldElement oY {}; //!< Y coordinate of the vector.
    };

    /**
     * @brief Operator to substract two Vector2D.
     * @param aFirst: A.
     * @param aSecond: B.
     * @return A - B.
     */
    template<typename tFieldElement>
    Vector2D<tFieldElement> operator-(const Vector2D<tFieldElement>& aFirst, const Vector2D<tFieldElement>& aSecond);

    // TODO: HK-47 Turn this into a template.
    /**
     * @brief Struct defining a three dimensional vector of f64.
     */
    struct Vector3D
    {
        /**
         * @brief Default constructor.
         */
        Vector3D() = default;

        /**
         * @brief Constructor from a single value.
         * @param aCommonValue: Value that all vector coordinates will have.
         */
        explicit Vector3D(f64 aCommonValue)
          : oX(aCommonValue)
          , oY(aCommonValue)
          , oZ(aCommonValue) {};

        /**
         * @brief Constructor.
         * @param aX: X dimension of the vector.
         * @param aY: Y dimesnion of the vector.
         * @param aZ: Z dimesnion of the vector.
         */
        Vector3D(f64 aX, f64 aY, f64 aZ)
          : oX(aX)
          , oY(aY)
          , oZ(aZ) {};

        // TODO: HK-47 Add comments to these.
        Vector3D        operator-() const { return {-oX, -oY, -oZ}; };
        friend Vector3D operator+(const Vector3D& aFirst, const Vector3D& aSecond) { return {aFirst.oX + aSecond.oX, aFirst.oY + aSecond.oY, aFirst.oZ + aSecond.oZ}; }
        friend Vector3D operator-(const Vector3D& aFirst, const Vector3D& aSecond) { return {aFirst.oX - aSecond.oX, aFirst.oY - aSecond.oY, aFirst.oZ - aSecond.oZ}; }
        friend Vector3D operator*(const Vector3D& aVector, f64 aScalar) { return {aVector.oX * aScalar, aVector.oY * aScalar, aVector.oZ * aScalar}; }
        friend Vector3D operator*(f64 aScalar, const Vector3D& aVector) { return aVector * aScalar; }
        friend Vector3D operator/(const Vector3D& aVector, f64 aScalar) { return (1.0 / aScalar) * aVector; }

        // TODO: Move this to a reasonable place.
        QColor ToQColor() const;

        f64 oX {0.0}; //!< X coordinate of the vector.
        f64 oY {0.0}; //!< Y coordinate of the vector.
        f64 oZ {0.0}; //!< Z coordinate of the vector.
    };

    /**
     * @brief Computes the dot product of twoo vectors.
     * @param aFirst: Vector A.
     * @param aSecond: Vector B.
     * @return A*B.
     */
    f64 DotProduct(const Vector3D& aFirst, const Vector3D& aSecond);

    /**
     * @brief Computes the cross product of two vectors.
     * @param aFirst: Vector A.
     * @param aSecond: Vector B.
     * @return AxB.
     */
    Vector3D CrossProduct(const Vector3D& aFirst, const Vector3D& aSecond);

    // TODO: HK-47: This should be a method of the vector.
    /**
     * @brief Returns the norm of a Vector3D.
     * @param aVector: The vector A.
     * @return ||A||.
     */
    f64 Norm(const Vector3D& aVector);

    // TODO: HK-47: This should be a method of the vector.
    /**
     * @brief Returns the direction of a Vector3D (the vector normalized)
     * @param aVector: The vector A.
     * @return A / ||A||.
     */
    Vector3D Normalize(const Vector3D& aVector);

    /**
     * @brief Moves a given point by a given amount along a direction.
     * @param aVector: The initial point O.
     * @param aDirection: The direction v.
     * @param aDistance: The amount d.
     * @return O + d*v.
     */
    Vector3D Translate(Vector3D& aPoint, const Vector3D& aDirection, f64 aDistance);

    // TODO: Could this be in a triangle struct?
    /**
     * @brief Returns a vector perpendicular to a given triangle. Vector modulus is the area of the triangle.
     * @param p1 A vertex of the triangle.
     * @param p2 A vertex of the triangle.
     * @param p3 A vertex of the triangle.
     * @return The normal to the triange.
     */
    inline Vector3D GetTrianglePerpendicular(const Vector3D& p0, const Vector3D& p1, const Vector3D& p2)
    {
        return 0.5 * CrossProduct(p1 - p0, p2 - p0);
    }

    /**
     * @brief Struct describing a 2D Box.
     */
    struct Box2D
    {
        /**
         * @brief Default constructor.
         */
        Box2D() = default;

        /**
         * @brief Constructor.
         * @param aMin: Minimum bounds point of the box.
         * @param aMax: Maximum bounds point of the box.
         */
        Box2D(const Vector2D<f64>& aMin, const Vector2D<f64>& aMax);

        Vector2D<f64> oMin {}; //!< Minimum bounds point.
        Vector2D<f64> oMax {}; //!< Maximum bounds point.
    };

    /**
     * @brief Struct describing a 3D Box.
     */
    struct Box3D
    {
        Vector3D oMin {}; //!< Minimum bounds point.
        Vector3D oMax {}; //!< Maximum bounds point.

        // TODO: Move this to the .h.

        /**
         * @brief Gets the center of the box.
         * @return The box's center.
         */
        Vector3D GetCenter() const { return 0.5 * (oMin + oMax); }

        /**
         * @brief Gets the box radius.
         * @retrun The distance between two opposing corners.
         */
        f64 GetRadius() const { return 0.5 * Math::Norm(oMin - oMax); }

        /**
         * @brief Get the height of the box.
         * @retrurn Max.X - Min.X.
         */
        f64 GetHeight() const { return oMax.oX - oMin.oX; }

        /**
         * @brief Get the height of the box.
         * @retrurn Max.Y - Min.Y.
         */
        f64 GetWidth() const { return oMax.oY - oMin.oY; }

        /**
         * @brief Get the height of the box.
         * @retrurn Max.Z - Min.Z.
         */
        f64 GetDepth() const { return oMax.oZ - oMin.oZ; }
    };
}

#include "Math.tpp"

#endif // MATH_H
