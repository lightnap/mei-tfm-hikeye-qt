#include "Math.hpp"

#include <QColor> //TODO: Deñete this.

#include <cmath>

namespace Math
{
    // TODO: Move this to a reasonable place.
    QColor Vector3D::ToQColor() const
    {
        return QColor(static_cast<s32>(255.0 * Math::Clamp(oX)), static_cast<s32>(255.0 * Math::Clamp(oY)), static_cast<s32>(255.0 * Math::Clamp(oZ)), 255);
    }

    f64 DotProduct(const Vector3D& aFirst, const Vector3D& aSecond)
    {
        return aFirst.oX * aSecond.oX + aFirst.oY * aSecond.oY + aFirst.oZ * aSecond.oZ;
    }

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

        if (VectorNorm == 0) [[unlikely]]
        {
            return {0.0, 0.0, 0.0};
        }
        else [[likely]]
        {
            return aVector / VectorNorm;
        }
    }

    Vector3D Translate(Vector3D& aPoint, const Vector3D& aDirection, f64 aDistance)
    {
        return aPoint + aDistance * aDirection;
    }

    Box2D::Box2D(const Vector2D<f64>& aMin, const Vector2D<f64>& aMax)
      : oMin(aMin)
      , oMax(aMax)
    {
    }

}
