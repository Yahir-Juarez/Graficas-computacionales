#include <iostream>
#include "Image.h"


int main()
{
	g_CodecMan.AddCodec(new BMPCodec());
	Image imagen;
	imagen.CreateFromImageFile("prueba3x3.bmp");
	imagen.Scale(2);

	auto codec = g_CodecMan.GetCodecByFileExt(".bmp");

	if (codec)
	{
		codec->Encode(imagen, "StarWrsScale2.bmp");
	}

	return 0;
}