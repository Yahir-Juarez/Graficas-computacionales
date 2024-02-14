#include <iostream>
#include <cmath>
#include "Image.h"


int main()
{
	g_CodecMan.AddCodec(new BMPCodec());
	Image imagen;
	imagen.CreateFromImageFile("StarWars.bmp");
	
	Image rotImg;

	float grados = 280;
	
	rotImg.rotate(grados, imagen);
	/*float primerPunto;
	float negativoX = 0;
	float negativoY = 0;
	float ancho;
	float alto;
	if (grados <= 90)
	{
		primerPunto = imagen.getRoationPosX(imagen.m_width, 0, grados);
		negativoX = imagen.getRoationPosX(0, imagen.m_height, grados);
		negativoX *= -1;
		ancho = (negativoX) + primerPunto;
		alto = imagen.getRoationPosY(imagen.m_width, imagen.m_height, grados);
	}
	else if (grados <= 180)
	{
		primerPunto = imagen.getRoationPosY(imagen.m_width, 0, grados);
		negativoY = imagen.getRoationPosY(0, imagen.m_height, grados);
		negativoY *= -1;
		alto = (negativoY) + primerPunto;
		negativoX = -1 * (imagen.getRoationPosX(imagen.m_width, imagen.m_height, grados));
		ancho = negativoX;
	}
	else if (grados <= 270)
	{
		primerPunto = imagen.getRoationPosX(0, imagen.m_height, grados);
		negativoX = imagen.getRoationPosX(imagen.m_width, 0, grados);
		negativoX *= -1;
		ancho = (negativoX)+primerPunto;
		negativoY = -1 * (imagen.getRoationPosY(imagen.m_width, imagen.m_height, grados));
		alto = negativoY;
	}
	else if (grados <= 360)
	{
		primerPunto = imagen.getRoationPosY(0, imagen.m_height, grados);
		negativoY = imagen.getRoationPosY(imagen.m_width, 0, grados);
		negativoY *= -1;
		alto = (negativoY)+primerPunto;
		ancho = imagen.getRoationPosX(imagen.m_width, imagen.m_height, grados);
	}
	rotImg.CreateImage(ancho, alto, imagen.m_bpp);

	for (int x = 1; x < imagen.m_width; x++)
	{
		for (int y = 1; y < imagen.m_height; y++)
		{
			Color colorInBuffer = imagen.GetPixel(x, y);
			rotImg.SetPixel((rotImg.getRoationPosX(x, y, grados) + negativoX) - 1, (rotImg.getRoationPosY(x, y, grados) + negativoY) - 1 , colorInBuffer);
		}
	}

	for (int x = 1; x < rotImg.m_width; x++)
	{
		for (int y = 1; y < rotImg.m_height; y++)
		{
			Color colorInBuffer = rotImg.GetPixel(x, y);
			if (colorInBuffer.isBlack())
			{
				colorInBuffer = rotImg.GetPixel(x + 1, y);
				if (!colorInBuffer.isBlack())
				{
					colorInBuffer = rotImg.GetPixel(x - 1, y);
					rotImg.SetPixel(x, y, colorInBuffer);
				}
			}
		}
	}*/

	//rotImg.SetPixel((rotImg.getRoationPosX(x, y, grados) + posPixel3 - posPixelX), rotImg.getRoationPosY(x, y, grados), colorInBuffer);


	/////////////////////////////////////////////////////// Escalar Imagen /////////////////////////////////////////////////////

	auto codec = g_CodecMan.GetCodecByFileExt(".bmp");

	if (codec)
	{
		codec->Encode(rotImg, "pruebaRotada.bmp");
	}

	return 0;
}