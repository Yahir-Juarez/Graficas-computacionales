#include <iostream>
#include <cmath>
#include "Image.h"

float lsR = 0.33f;
float lsG = 0.60f;
float lsB = 0.03f;

float leftSobel[9]  =  {-1, 0, 1,
					-2, 0, 2,
					-1, 0, 1};

float topSobel[9] = { 1, 2, 1,
				  0, 0, 0,
				  -1, -2, -1 };

float blur[9] = { 0.0625f, 0.125f, 0.0625f,
				0.125f, 0.25f, 0.125f,
				0.0625f, 0.125f, 0.0625f };

float outLine[9] = { -1, -1, -1,
					-1, 8, -1,
					-1, -1, -1 };

Color GreyScale(const Image& img, int pX, int pY)
{
	vector2D textCoorsSize(1.0f / img.m_width, 1.0f / img.m_height);
	Color currColor(img.SamplePixel(textCoorsSize.pointX * pX, textCoorsSize.pointY * pY));

	float lum = ((currColor.r * lsR) + (currColor.g * lsG) + (currColor.b * lsB));
	if (lum > 255)
	{
		lum = 255;
	}
	else if (lum < 0)
	{
		lum = 0;
	}
	return Color(lum, lum, lum);
}

Color sobelScale(const Image& img, int pX, int pY, const float matrix[])
{
	float sum = 0;
	sum += ((img.GetPixel(pX - 1, pY - 1)).r) * matrix[0];
	sum += ((img.GetPixel(pX, pY - 1)).r) * matrix[1];
	sum += ((img.GetPixel(pX + 1, pY - 1)).r) * matrix[2];
	sum += ((img.GetPixel(pX - 1, pY)).r) * matrix[3];
	sum += ((img.GetPixel(pX, pY)).r) * matrix[4];
	sum += ((img.GetPixel(pX + 1, pY)).r) * matrix[5];
	sum += ((img.GetPixel(pX - 1, pY + 1)).r) * matrix[6];
	sum += ((img.GetPixel(pX, pY + 1)).r) * matrix[7];
	sum += ((img.GetPixel(pX + 1, pY + 1)).r) * matrix[8];

	/*if (sum < 50 && sum >= 0)
	{
		cout << sum << " X -> " << pX << " Y -> " << pY << "\n";
		return Color(50, 30, 255);
	}*/
	if (sum > 255)
	{
		sum = 255;
	}
	else if (sum < 0)
	{
		sum = 0;
	}
	return Color(sum, sum, sum);
}
//Color blurScale(const Image& img, int pX, int pY)
//{
//	float sum = 0;
//	float* matrix = blur;
//	sum += ((img.GetPixel(pX - 1, pY - 1)).r) * matrix[0];
//	sum += ((img.GetPixel(pX, pY - 1)).r) * matrix[1];
//	sum += ((img.GetPixel(pX + 1, pY - 1)).r) * matrix[2];
//	sum += ((img.GetPixel(pX - 1, pY)).r) * matrix[3];
//	sum += ((img.GetPixel(pX, pY)).r) * matrix[4];
//	sum += ((img.GetPixel(pX + 1, pY)).r) * matrix[5];
//	sum += ((img.GetPixel(pX - 1, pY + 1)).r) * matrix[6];
//	sum += ((img.GetPixel(pX, pY + 1)).r) * matrix[7];
//	sum += ((img.GetPixel(pX + 1, pY + 1)).r) * matrix[8];
//
//	/*if (sum < 50 && sum >= 0)
//	{
//		cout << sum << " X -> " << pX << " Y -> " << pY << "\n";
//		return Color(50, 30, 255);
//	}*/
//
//	return Color(sum, sum, sum);
//}

int main()
{
	g_CodecMan.AddCodec(new BMPCodec());
	Image imagen;
	imagen.CreateFromImageFile("StarWars.bmp");
	//.Image scale;
	//scale.scaleImg(2, imagen);
	//Image imgRotate;
	//imgRotate.CreateFromImageFile("StarWars.bmp");
	//Image rotate;
	//rotate.rotate(197, imgRotate);
	//scale.bitBltImgRotate(rotate, 50, 50);
	imagen = imagen.ProcessImage(GreyScale);
	//imagen = imagen.ProcessImage(sobelScale, blur);
	imagen = imagen.ProcessImage(sobelScale, outLine);
	
	
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
		codec->Encode(imagen, "StarWarsGray.bmp");
	}

	return 0;
}