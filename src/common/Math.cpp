#include "Math.hpp"

#include <cmath>

namespace Math
{

// TODO: Maybe make this methods of the vector?
f64 DotProduct(const Vector3D& aFirst, const Vector3D& aSecond)
{
    return aFirst.oX * aSecond.oX + aFirst.oY * aSecond.oY + aFirst.oZ * aSecond.oZ;
}

// TODO: Maybe make these methods of the vector?
Vector3D CrossProduct(const Vector3D& aFirst, const Vector3D& aSecond)
{
    const f64 CrossX {aFirst.oY * aSecond.oZ - aFirst.oZ * aSecond.oY};
    const f64 CrossY {aFirst.oX * aSecond.oZ - aFirst.oZ * aSecond.oX};
    const f64 CrossZ {aFirst.oX * aSecond.oY - aFirst.oY * aSecond.oX};

    return {CrossX, -CrossY, CrossZ};
}

f64 Norm(const Vector3D& aVector)
{
    return std::sqrt(Math::DotProduct(aVector, aVector));
}

Vector3D Normalize(const Vector3D& aVector)
{
    const f64 VectorNorm {Norm(aVector)};

    return aVector / VectorNorm;
}

Vector3D Translate(Vector3D& aPoint, const Vector3D& aDirection, f64 aDistance)
{
    return aPoint + aDistance * aDirection;
}
}
