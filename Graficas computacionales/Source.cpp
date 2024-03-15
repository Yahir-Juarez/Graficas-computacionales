#include <iostream>
#include <random>
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
	std::random_device rd;  // Obtener una semilla aleatoria del dispositivo
	std::mt19937 gen(rd()); // Inicializar el generador con la semilla

	// Definir el rango de n�meros aleatorios
	int min = 1;
	int max = 1080;

	// Crear una distribuci�n uniforme dentro del rango especificado
	std::uniform_int_distribution<> distribucion(min, max);

	// Generar un n�mero aleatorio
	int numero_aleatorio = distribucion(gen);

	g_CodecMan.AddCodec(new BMPCodec());


	/////////////// Crear Imagen //////////////////////////

	Image imagenOriginal;
	Image imageOriginalRaster;
	imagenOriginal.CreateFromImageFile("StarWars.bmp");
	imageOriginalRaster.CreateFromImageFile("Yoda.bmp");

	///////////////// Escalar Imagen //////////////////////

	Image imageScale;
	imageScale.scaleImg(2.5, imagenOriginal);

	//////////////// bitBlit ////////////////////////

	Image imagebBitBlit;
	imagebBitBlit.CreateImage(1920, 1080, imagenOriginal.m_bpp);
	for (int i = 0; i < 200; i++)
	{
		imagebBitBlit.bitBlt(imagenOriginal, 200 + i, 200 - i, 200 - i , 200 - i);
	}

	////////////// Rotar Imagen ///////////////////////////	
	
	Image imageRotate;
	Image imageRasterRotate;
	imageRotate.rotate(127, imageScale);
	imageRasterRotate.rotate(237, imageOriginalRaster);

	///////////// Raster ////////////////////////////////

	Image imageRaster;
	imageRaster = imageScale;
	imageRaster.Raster(imageRasterRotate, 95, 123);

	////////////// Scale Gray ///////////////////////////

	Image processImageGray;
	processImageGray = imagenOriginal.ProcessImage(GreyScale);

	///////////// Process Blur //////////////////////////////

	Image processImageBlur;
	processImageBlur = processImageGray.ProcessImage(sobelScale, blur);

	///////////// Process sobel //////////////////////////////

	Image processImageSobel;
	processImageSobel = processImageGray.ProcessImage(sobelScale, leftSobel);

	
	auto codec = g_CodecMan.GetCodecByFileExt(".bmp");

	if (codec)
	{
		codec->Encode(imagenOriginal, "ImageProcess/StarWars.bmp");
		codec->Encode(imageScale, "ImageProcess/StarWarsScale.bmp");
		codec->Encode(imagebBitBlit, "ImageProcess/StarWarsBitBlit.bmp");
		codec->Encode(imageRotate, "ImageProcess/StarWarsRotate.bmp");
		codec->Encode(imageRaster, "ImageProcess/StarWarsRaster.bmp");
		codec->Encode(processImageGray, "ImageProcess/StarWarsGrayScale.bmp");
		codec->Encode(processImageBlur, "ImageProcess/StarWarsBlur.bmp");
		codec->Encode(processImageSobel, "ImageProcess/StarWarsSobel.bmp");
	}

	return 0;
}