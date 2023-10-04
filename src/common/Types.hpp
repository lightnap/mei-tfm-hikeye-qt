#ifndef TYPES_H
#define TYPES_H

#include <cstdint>
#include <string>

using u8 = uint8_t;   //!< Type for unsigned integer of 1 byte.
using u16 = uint16_t; //!< Type for unsigned integer of 2 bytes.
using u32 = uint32_t; //!< Type for unsigned integer of 4 bytes.
using u64 = uint64_t; //!< Type for unsigned integer of 8 bytes.

using s8 = int8_t;   //!< Type for signed integer of 1 byte.
using s16 = int16_t; //!< Type for signed integer of 2 bytes.
using s32 = int32_t; //!< Type for signed integer of 4 bytes.
using s64 = int64_t; //!< Type for signed integer of 8 bytes.

using f32 = float;  //!< Type for floating point number of 4 bytes.
using f64 = double; //!< Type for floating point number of 8 bytes.

namespace Types
{
/**
 * @brief Enum describing possible Loading Module types.
 */
enum class eLoadingModule
{
    Terrain = 0, //!< Loading module that loads terrain info.
    Tracks,      //!< Loading module that loads tack info.
    Size         //!< Size of this enum.
};

/**
 * @brief Enum describing the several resources to be loaded.
 */
enum class eResource
{
    None = 0,    //!< No resource to be loaded, or empty resource.
    HeightMap,   //!< Image file with terrain height data.
    Terrain,     //!< Terrain 3d model.
    GroundTruth, //!< Graph of tracks we consider as the ground truth.
    Matches,     //!< Recorded tracks we want to analyze.
    Queries,     //!< Ground Truth enriched with info from tracks.
    Texture,     //!< Image containing the queries info, to be displyed on map.
    Size         //!< Size of this enum.
};

/**
 * @brief Enum describing possible errors while loading resource.
 */
enum class eLoadResult
{
    Successful = 0, //!< No error while loading.
    Interrupted,    //!< Load interrupted by user.
    Error,          //!< Load stopped due to error.
    Size            //!< Size of this enum.
};

/**
 * @brief Reflection for the eResource Enum.
 * @param aModuleType Moduletype enum we want to turn into string.
 * @retrun String version of the Moduletype enum.
 */
std::string ResourceToString(Types::eResource aResource);

/**
 * @brief Reflection for the eModuleType Enum.
 * @param aModuleType eModuletype enum we want to turn into string.
 * @retrun String version of the Moduletype enum.
 */
std::string LoadingModuleToString(Types::eLoadingModule aModule);
}

#endif // TYPES_H
