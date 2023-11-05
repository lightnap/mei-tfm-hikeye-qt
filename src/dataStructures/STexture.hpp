#ifndef S_TEXTURE_H
#define S_TEXTURE_H

#include <QImage>

/**
 * @brief Struct that represents the texture to be drawn on the terrain.
 */
struct STexture
{
  public:
    /**
     * @brief Constructor.
     * @param aTexture: QImage that will be our texture.
     */
    STexture(QImage&& aTexture);

    QImage oTexture; //!< Texture we draw on the terrain.
};

#endif // S_TEXTURE_H
