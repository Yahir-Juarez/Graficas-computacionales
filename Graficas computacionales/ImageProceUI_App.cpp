#include "ImageProceUI_App.h"

Image
ImageProceUI_APP::resolveImageKebler(Image& image, KEBLERS::E kebler, int repetitionKebler)
{
	Image imageExport;
  switch (kebler)
  {
  case KEBLERS::kBlur:
    imageExport = image.ProcessImageColor(KeblerImage::blur);

    for (int i = 1; i < repetitionKebler; i++) {
      imageExport = imageExport.ProcessImageColor(KeblerImage::blur);
    }
    break;
	case KEBLERS::kOutline:
		imageExport = image.ProcessImage(KeblerImage::outLine);
		break;
	case KEBLERS::kLeftSobel:
		imageExport = image.ProcessImage(KeblerImage::leftSobel);
		break;
	case KEBLERS::kTopSobel:
		imageExport = image.ProcessImage(KeblerImage::topSobel);
		break;
	case KEBLERS::kSumSobel:
	{
		Image processImageSobel;
		processImageSobel = image.ProcessImage(KeblerImage::leftSobel);

		imageExport = image.ProcessImage(KeblerImage::topSobel);
		for (int x = 0; x < imageExport.m_height; x++)
		{
			for (int y = 0; y < imageExport.m_width; y++)
			{
				if (!processImageSobel.GetPixel(x, y).isBlack())
				{
					Color newColorLeft;
					Color neColorTop;
					newColorLeft = processImageSobel.GetPixel(x, y);
					neColorTop = imageExport.GetPixel(x, y);
					newColorLeft.r += neColorTop.r;
					newColorLeft.g += neColorTop.r;
					newColorLeft.b += neColorTop.r;
					if (newColorLeft.r > 255)
					{
						newColorLeft.r = 255;
						newColorLeft.g = 255;
						newColorLeft.b = 255;
					}
					imageExport.SetPixel(x, y, newColorLeft);
				}
			}
		}
	}
		break;
	case KEBLERS::kLeftPrewwit:
		imageExport = image.ProcessImage(KeblerImage::leftPrewwit);
		break;
	case KEBLERS::kTopPrewwit:
		imageExport = image.ProcessImage(KeblerImage::topPrewwit);
		break;
	default:
		break;
	}
  return imageExport;
}
