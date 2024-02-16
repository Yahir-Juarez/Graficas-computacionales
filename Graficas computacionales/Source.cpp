#include <iostream>
#include <cmath>
#include "Image.h"


int main()
{
	g_CodecMan.AddCodec(new BMPCodec());
	Image imagen;
	imagen.CreateFromImageFile("StarWars.bmp");
	
	Image BBLImg;
	//BBLImg;
	//BBLImg.bitBlt(imagen, 100);
	BBLImg.rotate(80, imagen);
	/*Image rotImg;

	float grados = 33;
	
	rotImg.rotate(grados, imagen);*/
	

	//rotImg.SetPixel((rotImg.getRoationPosX(x, y, grados) + posPixel3 - posPixelX), rotImg.getRoationPosY(x, y, grados), colorInBuffer);


	auto codec = g_CodecMan.GetCodecByFileExt(".bmp");

	if (codec)
	{
		codec->Encode(BBLImg, "ImgPegada.bmp");
	}

	return 0;
}