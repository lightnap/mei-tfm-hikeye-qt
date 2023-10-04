#include "CCamera.hpp"

#include <QMatrix4x4> // TODO: Consider using glm library.
#include <QVector3D>  // TODO: Consider using glm library.

#include <cmath>

CCamera::CCamera()
  : mAspectRatio(1.0)
  , mFieldOfView(60.0)
  , mInitialFieldOfView(60.0)
  , mNearPlane(1.0)
  , mFarPlane(1000.0)
  , mEye(0.0, 0.0, 0.0)
  , mAt(0.0, 1.0, 0.0)
  , mUp(0.0, 0.0, 1.0) {};

Math::Vector3D CCamera::GetViewDirection()
{
    return Math::Normalize(mAt - mEye);
}

// TODO: Fix these so that they work even upside down (maybe quaternions?)
void CCamera::LookAt(const Math::Box3D& aBox)
{
    Math::Vector3D MinToCenter {0.5 * (aBox.oMax - aBox.oMin)};
    Math::Vector3D MinToBaseCenter {MinToCenter.oX, MinToCenter.oY, 0.0};
    const f64      Radius = Math::Norm(MinToBaseCenter);
    Math::Vector3D CameraEyeOffset {2 * MinToBaseCenter.oX, 2 * MinToBaseCenter.oY, -Radius};

    mEye = aBox.GetCenter() - CameraEyeOffset;
    mAt = aBox.GetCenter();
    mUp = Math::Vector3D(0.0, 0.0, 1.0);
    mNearPlane = Radius;
    mFarPlane = 3 * Radius;
}

void CCamera::VerticalRotateArroundAt(f64 aAngle)
{
    const Math::Vector3D ViewDirection {mAt - mEye};
    f64                  ViewDirectionNorm {Math::Norm(ViewDirection)};
    const Math::Vector3D NormalizedViewDirection {ViewDirection / ViewDirectionNorm};
    const Math::Vector3D Left {Math::Normalize(Math::CrossProduct(mUp, ViewDirection))};

    const Math::Vector3D NewViewDirection {NormalizedViewDirection * std::cos(aAngle) + mUp * std::sin(aAngle)};

    mUp = Math::CrossProduct(NewViewDirection, Left);
    mEye = Math::Translate(mAt, NewViewDirection, -ViewDirectionNorm);
}

void CCamera::HorizontalRotateArroundAt(f64 aAngle)
{
    const Math::Vector3D ViewDirection {mEye - mAt};
    const Math::Vector3D Left {Math::CrossProduct(mUp, ViewDirection)};

    const f64 Sin {std::sin(aAngle)};
    const f64 Cos {std::cos(aAngle)};

    const f64 NewViewDirectionX {Cos * ViewDirection.oX - Sin * ViewDirection.oY};
    const f64 NewViewDirectionY {Sin * ViewDirection.oX + Cos * ViewDirection.oY};

    const Math::Vector3D NewViewDirection {NewViewDirectionX, NewViewDirectionY, ViewDirection.oZ};

    const f64 NewLeftX {Cos * Left.oX - Sin * Left.oY};
    const f64 NewLeftY {Sin * Left.oX + Cos * Left.oY};

    const Math::Vector3D NewLeft {NewLeftX, NewLeftY, 0.0};
    mUp = Math::Normalize(Math::CrossProduct(NewLeft, -NewViewDirection));
    mEye = mAt + NewViewDirection;
}

void CCamera::RadialTranslate(f64 aDistance, bool aMoveAt)
{
    const Math::Vector3D ViewDirection {GetViewDirection()};
    mEye = Math::Translate(mEye, ViewDirection, aDistance);

    if (aMoveAt)
    {
        mAt = Math::Translate(mAt, ViewDirection, aDistance);
    }
}

void CCamera::VerticalTranslate(f64 aDistance)
{
    const Math::Vector3D ViewDirection {GetViewDirection()};
    const Math::Vector3D Left {Math::Normalize(Math::CrossProduct(Math::Vector3D(0.0, 0.0, 1.0), ViewDirection))};

    mEye = mEye + aDistance * Math::CrossProduct(ViewDirection, Left);
    mAt = mAt + aDistance * Math::CrossProduct(ViewDirection, Left);
}

void CCamera::HorizontalTranslate(f64 aDistance)
{
    Math::Vector3D ViewDirection {mAt - mEye};
    ViewDirection.oZ = 0.0;
    ViewDirection = Math::Normalize(ViewDirection);
    const Math::Vector3D Left {Math::Normalize(Math::CrossProduct(Math::Vector3D(0.0, 0.0, 1.0), ViewDirection))};

    mEye = mEye + aDistance * Left;
    mAt = mAt + aDistance * Left;
}

void CCamera::UpdateAspectRatio(f64 aNewRatio)
{
    mAspectRatio = aNewRatio;

    if (aNewRatio < 1.0)
    {
        const f64 ViewAngleTangent {std::tan(Math::DegToRad(mInitialFieldOfView / 2.0))};
        mFieldOfView = Math::RadToDeg(2.0 * std::atan(ViewAngleTangent / aNewRatio));
    }
    else
    {
        mFieldOfView = mInitialFieldOfView;
    }
}

QMatrix4x4 CCamera::GetViewMatrix()
{
    // TODO: Consider using glm library.
    QMatrix4x4      ViewMatrix;
    const QVector3D Eye(mEye.oX, mEye.oY, mEye.oZ);
    const QVector3D At(mAt.oX, mAt.oY, mAt.oZ);
    const QVector3D Up(mUp.oX, mUp.oY, mUp.oZ);

    ViewMatrix.lookAt(Eye, At, Up);
    return ViewMatrix;
}

QMatrix4x4 CCamera::GetProjectionMatrix()
{
    // TODO: Consider using glm library.
    QMatrix4x4 ProjectionMatrix;
    ProjectionMatrix.perspective(mFieldOfView, mAspectRatio, mNearPlane, mFarPlane);
    return ProjectionMatrix;
}
