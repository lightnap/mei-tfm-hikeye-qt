#include "STexture.hpp"

#include <QImage>

#include <utility>

STexture::STexture(QImage&& aTexture)
  : oTexture(std::move(aTexture))
{
}
