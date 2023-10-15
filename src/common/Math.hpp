#ifndef MATH_H
#define MATH_H

#include "Types.hpp"
#include <cmath>

/**
 * @brief Math functions.
 */
// TODO: Change this namespace to math.
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

constexpr f64 PI = 3.14159265358979323846;

inline f64 DegToRad(f64 aDegree)
{
    return (PI / 180.0) * aDegree;
}

inline f64 RadToDeg(f64 aRadian)
{
    return (180.0 / PI) * aRadian;
}

// TODO: Turn this into a template.
/**
 * @brief Struct defining a two dimensional vector of f64.
 */
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
    Vector2D(f64 aX, f64 aY)
      : oX(aX)
      , oY(aY) {};

    f64 oX {0.0}; //!< X coordinate of the vector.
    f64 oY {0.0}; //!< Y coordinate of the vector.
};

// TODO: Turn this into a template.
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

    // TODO: Add comments to these.
    Vector3D        operator-() const { return {-oX, -oY, -oZ}; };
    friend Vector3D operator+(const Vector3D& aFirst, const Vector3D& aSecond) { return {aFirst.oX + aSecond.oX, aFirst.oY + aSecond.oY, aFirst.oZ + aSecond.oZ}; }
    friend Vector3D operator-(const Vector3D& aFirst, const Vector3D& aSecond) { return {aFirst.oX - aSecond.oX, aFirst.oY - aSecond.oY, aFirst.oZ - aSecond.oZ}; }
    friend Vector3D operator*(const Vector3D& aVector, f64 aScalar) { return {aVector.oX * aScalar, aVector.oY * aScalar, aVector.oZ * aScalar}; }
    friend Vector3D operator*(f64 aScalar, const Vector3D& aVector) { return aVector * aScalar; }
    friend Vector3D operator/(const Vector3D& aVector, f64 aScalar) { return (1.0 / aScalar) * aVector; }

    f64 oX {0.0}; //!< X coordinate of the vector.
    f64 oY {0.0}; //!< Y coordinate of the vector.
    f64 oZ {0.0}; //!< Z coordinate of the vector.
};

// TODO: Maybe make this methods of the vector?
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

/**
 * @brief Returns the norm of a Vector3D.
 * @param aVector: The vector A.
 * @return ||A||.
 */
f64 Norm(const Vector3D& aVector);

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

/**
 * @brief Struct describing a 2D Box.
 */
struct Box2D
{
    Vector2D oMin {}; //!< Minimum bounds point.
    Vector2D oMax {}; //!< Maximum bounds point.
};

/**
 * @brief Struct describing a 3D Box.
 */
struct Box3D
{
    Vector3D oMin {}; //!< Minimum bounds point.
    Vector3D oMax {}; //!< Maximum bounds point.

    /**
     * @brief Gets the center of the box.
     * @return The box's center.
     */
    Vector3D GetCenter() const { return 0.5 * (oMin + oMax); }
};
}

#endif // MATH_H