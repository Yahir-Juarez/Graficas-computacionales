#include <iostream>
#include "Image.h"


int main()
{
	g_CodecMan.AddCodec(new BMPCodec());
	Image imagen;
	imagen.CreateFromImageFile("StarWars.bmp");

	float imgScale = 3.5f;
	Image scaleImage;

	//scaleImage.scaleImg2(imgScale, imagen);

	/////////////////////////////////////////////////////// Escalar Imagen /////////////////////////////////////////////////////

	scaleImage.CreateImage(imagen.m_width * imgScale, imagen.m_height * imgScale, imagen.m_bpp);

	float siU = 1.0f / scaleImage.m_width;
	float siV = 1.0f / scaleImage.m_height;

	for (int y = 0; y < scaleImage.m_height; ++y)
	{
		for (int x = 0; x < scaleImage.m_width; x++)
		{
			scaleImage.SetPixel(x, y, imagen.SamplePixel(siU * x, siV * y));
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	auto codec = g_CodecMan.GetCodecByFileExt(".bmp");

	if (codec)
	{
		codec->Encode(scaleImage, "StarWrsScale5.bmp");
	}

	return 0;
}