#include <iostream>
#include <cmath>
#include "Image.h"

float lsR = 0.55f;
float lsG = 0.60f;
float lsB = 0.03f;

Color GreyScale(const Image& img, int pX, int pY)
{
	vector2D textCoorsSize(1.0f / img.m_width, 1.0f / img.m_height);
	Color currColor(img.SamplePixel(textCoorsSize.pointX * pX, textCoorsSize.pointY * pY));

	float lum = (currColor.r * lsR + currColor.g * lsG + currColor.b * lsB);
	return Color(lum, lum, lum);
}

int main()
{
	g_CodecMan.AddCodec(new BMPCodec());
	Image imagen;
	imagen.CreateFromImageFile("Yoda.bmp");
	Image scale;

	imagen = imagen.ProcessImage(GreyScale);
	//scale.scaleImg(4, imagen);
	////ImagenRotate.draw_line(ImagenRotate.getPoint1().pointX, ImagenRotate.getPoint1().pointY, ImagenRotate.getPoint3().pointX, ImagenRotate.getPoint3().pointY);
	//Image BBLImg;
	////BBLImg.CreateImage(1920, 1080, imagen.m_bpp);
	////BBLImg.CreateImage(1920, 1080, imagen.m_bpp);
	//BBLImg.CreateFromImageFile("StarWars.bmp");
	//Image ImagenRotate;
	//ImagenRotate.rotate(67, BBLImg);
	////Image scaled;
	////Image ImgRotate;
	////scaled.scaleImg(2, BBLImg);
	//scale.bitBltImgRotate(ImagenRotate, 50, 50);
	//BBLImg.bitBltImgRotate(ImagenRotate, 120, 300);
	//ImgRotate.rotate(25, imagen);
	//scaled.bitBlt(ImgRotate, 150);
	//scaled.DrawLine(1, 5, 9, 1331, Color(255, 0, 0));
	//BBLImg;
	//BBLImg.bitBlt(imagen, 100);
	//BBLImg.rotate(80, imagen);
	/*Image rotImg;

	float grados = 33;
	
	rotImg.rotate(grados, imagen);*/
	

	//rotImg.SetPixel((rotImg.getRoationPosX(x, y, grados) + posPixel3 - posPixelX), rotImg.getRoationPosY(x, y, grados), colorInBuffer);

	/*for (int i = 0; i <= 4; i++)
	{
		ImagenRotate.SetPixel(i, 1, Color(0, 0, 255));
	}*/
	auto codec = g_CodecMan.GetCodecByFileExt(".bmp");

	if (codec)
	{
		codec->Encode(imagen, "ImgPegada.bmp");
	}

	return 0;
}