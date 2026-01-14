#pragma once

#include "Image.h"
#include "Utilities.h"

namespace KEBLERS {
  enum E {
    kBlur = 0,
    kOutline,
    kLeftSobel,
    kTopSobel,
    kSumSobel,
    kLeftPrewwit,
    kTopPrewwit,
  };
};

class ImageProceUI_APP
{
 public:
  static Color
  greyScale(const Image& img, int pX, int pY)
  {
    vector2D textCoorsSize(1.0f / img.m_width, 1.0f / img.m_height);
    Color currColor(img.SampleBilineal(textCoorsSize.pointX * pX, textCoorsSize.pointY * pY));

    float lum = ((currColor.r * ConfigImage::lsR) +
      (currColor.g * ConfigImage::lsG) +
      (currColor.b * ConfigImage::lsB));

    if (lum > 255.0f)
    {
      lum = 255.0f;
    }
    else if (lum < 0.0f)
    {
      lum = 0.0f;
    }
    return Color(lum, lum, lum);
  }

  static Image
  resolveImageKebler(Image& image, KEBLERS::E kebler, int repetKebler = 1);
};
