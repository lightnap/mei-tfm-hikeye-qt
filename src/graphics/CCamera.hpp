#ifndef C_CAMERA_H
#define C_CAMERA_H

#include "Math.hpp"
#include "Types.hpp"

#include <QMatrix4x4> //TODO: HK-47 Consider using the glm library.

/**
 * @brief Class depicting a camera.
 */
class CCamera
{
  public:
    /**
     * @brief Constructor.
     */
    CCamera();

    /**
     * @brief Returns the (normalized) direction the camera in pointing towards.
     * @return Norm(At - Eye).
     */
    Math::Vector3D GetViewDirection();

    /**
     * @brief Points the camera toward the center of a Box3D.
     * @param aBox: The Box3D we want to look at.
     */
    void LookAt(const Math::Box3D& aBox);

    /**
     * @brief Gets the the eye of the camera.
     * @return The eye of the camera.
     */
    Math::Vector3D GetEye() const { return mEye; }

    /**
     * @brief Gets the At point; that is, the point the camera is looking at.
     * @return The point the camera is looking at.
     */
    Math::Vector3D GetAt() const { return mAt; }

    // TODO: HK-43 Delete these.
    // Math::Vector3D getUp() const { return mUp; }
    // double    getNearPlane() const { return mNearPlane; }
    // double    getFarPlane() const { return mFarPlane; }
    // double    getAngleOfViewH(double, double) const;
    // double    getAngleOfViewV(double, double) const;

    // TODO: HK-43 This should not be necessary.
    /**
     * @brief Sets the point the camera is looking at.
     * @param aAt: The point we want the camera to look at.
     */
    void SetAt(const Math::Vector3D& aAt)
    {
        mAt = aAt;
        mUp = Math::Vector3D {0.0, 0.0, 1.0};
    }

    // TODO: HK-43 This should not be necessary.
    /**
     * @brief Sets the camera position.
     * @param aEye: The new camera position.
     */
    void SetEye(const Math::Vector3D& aEye) { mEye = aEye; }

    /**
     * @brief Moves camera around eye, in the vertical position.
     * @param aAngle: Amount that we move.
     */
    void VerticalRotateArroundAt(f64 aAngle);

    /**
     * @brief Moves camera around eye, in the horizontal position.
     * @param aAngle: Amount that we move.
     */
    void HorizontalRotateArroundAt(f64 aAngle);

    /**
     * @brief Moves camera towards the At point.
     * @param aDistance: Amount we move.
     * @param aMoveAt: true if we want to move the at point too, false if we move only eye.
     */
    void RadialTranslate(f64 aDistance, bool aMoveAt = false);

    /**
     * @brief Move the camera along the up direction.
     * @param aDistance: Amount we want to move.
     */
    void VerticalTranslate(f64 aDistance);

    /**
     * @brief Changes the aspect ratio of the camera to the new designated one.
     * @param aNewRatio: The new aspect ratio of the camera.
     */
    void UpdateAspectRatio(f64 aNewRatio);

    /**
     * @brief Move the camera along the horizontal direction.
     * @param aDistance: Amount we want to move.
     */
    void HorizontalTranslate(f64 aDistance);

    /**
     * @brief Gets the view matrix for this camera.
     * @return The view matrix for this camera.
     */
    QMatrix4x4 GetViewMatrix();

    /**
     * @brief Gets the projection matrix for this camera.
     * @return The view matrix for this camera.
     */
    QMatrix4x4 GetProjectionMatrix();

  private:
    f64 mAspectRatio;        //!< Height of the viewfiled divided by its width.
    f64 mFieldOfView;        //!< Current camera field of view (in degrees).
    f64 mInitialFieldOfView; //!< Camera field of view for aspect ratio of 1.
    f64 mNearPlane;          //!< Distance from the eye to the near plane.
    f64 mFarPlane;           //!< Distance from the eye to the far plane.

    Math::Vector3D mEye; //!< Point from which the image is taken.
    Math::Vector3D mAt;  //!< Point at which the camera is looking.
    Math::Vector3D mUp;  //!< Up direction for the camera.
};

#endif // C_CAMERA_H
