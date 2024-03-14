#ifndef C_CUSTOM_COLOUR_SPECTRUM_H
#define C_CUSTOM_COLOUR_SPECTRUM_H

#include "common/Math.hpp"
#include "common/Types.hpp"

#include <vector>

/**
 * @brief Class that represents a spectrum of colors.
 */
class CCustomColourSpectrum
{
  public:
    /**
     * @brief Constructor.
     * @param aColors: Pure colors that will form the spectrum.
     * @param aAnchors: Wavelength values in which the given colors are purest.
     */
    CCustomColourSpectrum(const std::vector<Math::Vector3D>& aColors, const std::vector<f64>& aAnchors);

    /**
     * @brief Gets a color in the spectrum, given a wavelength.
     * @param aWavelength: The wavelength.
     * @return The color of this wavelength in this spectrum.
     */
    Math::Vector3D GetColor(f64 aWavelength) const;

    // TODO: Hk-53 This does not go here. Move to a ColorSpectrumPrototypes file where they are data and not methods, when we need more of them.
    /**
     * @brief Gets the color spectrum that we use for reliefs.
     * @return The relief color spectrum.
     */
    static CCustomColourSpectrum Relief();

    /**
     * @brief Gets the color spectrum that we use for heatmaps. Based on https://www.kennethmoreland.com/color-advice/.
     * @return The heatmap color spectrum.
     */
    static CCustomColourSpectrum CoolWarm();

    /**
     * @brief Gets the color spectrum that we use for symmetric colorings. Based on https://colorbrewer2.org/.
     * @return The symmetric color spectrum.
     */
    static CCustomColourSpectrum Symmetric();

  protected:
    std::vector<Math::Vector3D> mColors;  //!< List of colors in this spectrum.
    std::vector<f64>            mAnchors; //!< Points in the spectrum where each color will be purest.
};

#endif // C_CUSTOM_COLOUR_SPECTRUM_H
