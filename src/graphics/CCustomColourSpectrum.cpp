#include "CCustomColourSpectrum.hpp"

#include "common/Math.hpp"
#include "common/Types.hpp"

CCustomColourSpectrum::CCustomColourSpectrum(const std::vector<Math::Vector3D>& aColors, const std::vector<f64>& aAnchors)
  : mColors(aColors)
  , mAnchors(aAnchors)
{
}

Math::Vector3D CCustomColourSpectrum::GetColor(f64 aWavelength) const
{
    if (mColors.size() == 0U)
    {
        return Math::Vector3D(1.0);
    }

    if (mColors.size() == 1U)
    {
        return mColors.at(0U);
    }

    if (aWavelength <= mAnchors.front())
    {
        return mColors.front();
    }

    if (aWavelength >= mAnchors.back())
    {
        return mColors.back();
    }

    for (u32 ColorIndex {0U}; ColorIndex < mColors.size() - 1U; ColorIndex++)
    {
        if (aWavelength < mAnchors.at(ColorIndex + 1))
        {
            f64 s = Math::LinearStep(aWavelength, mAnchors.at(ColorIndex), mAnchors.at(ColorIndex + 1));
            return (1 - s) * mColors.at(ColorIndex) + s * mColors.at(ColorIndex + 1);
        }
    }

    return Math::Vector3D(1.0);
}

CCustomColourSpectrum CCustomColourSpectrum::Relief()
{
    const std::vector<Math::Vector3D> Colours = {Math::Vector3D(160, 220, 105) / 255.0,
                                                 Math::Vector3D(1.0, 0.9, 0.45),
                                                 Math::Vector3D(168 / 255.0, 155 / 255.0, 138 / 255.0),
                                                 Math::Vector3D(0.95, 0.95, 0.95)};

    const std::vector<f64> Anchors {0.0, 150.0 / 400.0, 250.0 / 400.0, 400.0 / 400.0};

    return CCustomColourSpectrum(Colours, Anchors);
}

CCustomColourSpectrum CCustomColourSpectrum::CoolWarm()
{
    const std::vector<Math::Vector3D> Colours = {
      Math::Vector3D(59, 76, 192) / 255.0,
      Math::Vector3D(221, 221, 221) / 255.0,
      Math::Vector3D(180, 4, 38) / 255.0,
    };

    const std::vector<f64> Anchors {0.0, 0.5, 1.0};

    return CCustomColourSpectrum(Colours, Anchors);
}
