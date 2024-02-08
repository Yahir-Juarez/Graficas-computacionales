#include <iostream>
#include <cmath>
#include "Image.h"


int main()
{
	g_CodecMan.AddCodec(new BMPCodec());
	Image imagen;
	imagen.CreateFromImageFile("StarWars.bmp");
	
	Image rotImg;

	float posPixelX;
	float posPixelY;

	float grados = 60;
	
	float radianes = (grados * 3.1416) / 180;
	
	posPixelX = (imagen.m_width * cos(radianes));
	posPixelY = (imagen.m_height * cos(radianes) + imagen.m_width * sin(radianes));
	float posPixel3 = (-1 ) * (imagen.m_height * sin(radianes));

	posPixel3 *= -1;
	posPixel3 += posPixelX;

	rotImg.CreateImage(posPixel3, posPixelY, imagen.m_bpp);

	for (int x = 1; x < imagen.m_width; x++)
	{
		for (int y = 1; y < imagen.m_height; y++)
		{
			Color colorInBuffer = imagen.GetPixel(x, y);
			rotImg.SetPixel((rotImg.getRoationPosX(x, y, grados) + posPixel3 - posPixelX) - 1, (rotImg.getRoationPosY(x, y, grados)) - 1 , colorInBuffer);
		}
	}

	//rotImg.SetPixel((rotImg.getRoationPosX(x, y, grados) + posPixel3 - posPixelX), rotImg.getRoationPosY(x, y, grados), colorInBuffer);


	/////////////////////////////////////////////////////// Escalar Imagen /////////////////////////////////////////////////////

	/*
	float imgScale = 3.5f;
	Image scaleImage;
	scaleImage.CreateImage(imagen.m_width * imgScale, imagen.m_height * imgScale, imagen.m_bpp);

	float siU = 1.0f / scaleImage.m_width;
	float siV = 1.0f / scaleImage.m_height;

	for (int y = 0; y < scaleImage.m_height; ++y)
	{
		for (int x = 0; x < scaleImage.m_width; x++)
		{
			scaleImage.SetPixel(x, y, imagen.SamplePixel(siU * x, siV * y));
		}
	}*/

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	auto codec = g_CodecMan.GetCodecByFileExt(".bmp");

	if (codec)
	{
		codec->Encode(rotImg, "pruebaRotada.bmp");
	}

	return 0;
}