#include <iostream>
#include "Image.h"


int main()
{
	g_CodecMan.AddCodec(new BMPCodec());
	Image imagen;
	imagen.CreateFromImageFile("StarWars.bmp");

	auto codec = g_CodecMan.GetCodecByFileExt(".bmp");

	if (codec)
	{
		codec->EncodeScale(imagen, "StarWars2.bmp", 3);
	}

	return 0;
}