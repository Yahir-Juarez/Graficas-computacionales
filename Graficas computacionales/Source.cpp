#include <iostream>
#include <random>
#include <cmath>
#include <filesystem>
#include "Image.h"
#include "ImageProceUI_App.h"
#include <SFML/Graphics.hpp>

int main3()
{
	// Crear una ventana
	sf::VideoMode size = sf::VideoMode(sf::Vector2u(800, 600));

	sf::RenderWindow window(size, "Mi Ventana SFML");

	// Limitar el framerate (opcional)
	window.setFramerateLimit(60);

	// Crear un círculo para dibujar
	sf::CircleShape circle(50);
	circle.setFillColor(sf::Color::Green);
	circle.setPosition(sf::Vector2f(375.0f, 275.0f));

	// Loop principal
	while (window.isOpen())
	{
		//sf::Event event;
		//while (window.pollEvent(event))
		//{
		//	// Cerrar ventana con la X
		//	if (event.type == sf::Event::Closed)
		//		window.close();

		//	// Presionar ESC también cierra la ventana
		//	if (event.type == sf::Event::KeyPressed &&
		//		event.key.code == sf::Keyboard::Escape)
		//	{
		//		window.close();
		//	}
		//}

		// Limpiar, dibujar y mostrar
		window.clear(sf::Color(30, 30, 30)); // Fondo gris oscuro
		window.draw(circle);
		window.display();
	}

	return 0;
}

int Random(int min, int max) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dis(min, max);
  return dis(gen);
}

//Color sobelScale(const Image& img, int pX, int pY, const float matrix[])
//{
//	float sum = 0;
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
//	if (sum > 255)
//	{
//		sum = 255;
//	}
//	else if (sum < 0)
//	{
//		sum = 0;
//	}
//	return Color(sum, sum, sum);
//}
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

std::string GetFileName(const std::string& path)
{
	std::filesystem::path p(path);
	return p.stem().string(); // Devuelve el nombre sin la extensión
}

int main2()
{
	g_CodecMan.AddCodec(new BMPCodec());

	/////////////// Crear Imagen //////////////////////////

	cout << "Escribe una ruta a una imagen tipo BMP, la imagen debe ser RGB de 3 canales\n";
	cout << "Ejemplo:\nC:/Users/super/OneDrive/Escritorio/Graficas computacionales/x64/Release/Yoda.bmp\n";

	string path;
	std::getline(std::cin, path);

	Image imagenOriginal;
	Image imageOriginalRaster;
	imagenOriginal.CreateFromImageFile(path);
	imageOriginalRaster.CreateFromImageFile("Yoda.bmp");

	///////////////////////////////////////////////////////

	Image bilinealImage;
	bilinealImage.CreateImage(imagenOriginal.m_width, imagenOriginal.m_height, imagenOriginal.m_bpp);
	for (int iY = 0; iY < imagenOriginal.m_height; iY++)
	{
		for (int iX = 0; iX < imagenOriginal.m_width; iX++)
		{
			float siU = (1.0f / imagenOriginal.m_width) * iX;
			float siV = (1.0f / imagenOriginal.m_height) * iY;
			Color color(imagenOriginal.SampleBilineal(siU, siV));
			bilinealImage.SetPixel(iX, iY, color);
		}
	}


	///////////////// Escalar Imagen //////////////////////

	Image imageScale;
	imageScale.scaleImg(2.5, imagenOriginal);

	std::cout << "Imagen escalada\n";

	//////////////// bitBlit ////////////////////////
	Image imagebBitBlit;
	imagebBitBlit.CreateImage(1920, 1080, imagenOriginal.m_bpp);
	for (int i = 0; i < 1000; i++)
	{
		imagebBitBlit.bitBlt(imagenOriginal, Random(-1920, 1920), Random(-1080, 1080), 
			Random((0), imagenOriginal.m_width)),Random((0), imagenOriginal.m_height), 
			Random((0), imagenOriginal.m_width), Random((0), imagenOriginal.m_height);
	}

	std::cout << "Bit Blit\n";

	////////////// Rotar Imagen ///////////////////////////	
	
	Image imageRotate;
	Image imageRasterRotate;
	imageRotate.rotate(127, imageScale);
	imageRasterRotate.rotate(237, imageOriginalRaster);

	std::cout << "Imagen rotada\n";

	///////////// Raster ////////////////////////////////

	Image imageRaster;
	imageRaster = imageScale;
	imageRaster.Raster(imageRasterRotate, 95, 123);

	std::cout << "Raster\n";

	////////////// Scale Gray ///////////////////////////

	Image processImageGray;
	processImageGray = imagenOriginal.ProcessImage(ImageProceUI_APP::greyScale);

	std::cout << "Filtro blanco y negro\n";

	///////////// Process Blur //////////////////////////////

	Image processImageBlur;
	processImageBlur = imagenOriginal.ProcessImage(KeblerImage::blur);

	std::cout << "Blur\n";

	///////////// Process sobel //////////////////////////////

	Image processImageSobel;
	processImageSobel = processImageGray.ProcessImage(KeblerImage::leftSobel);

	Image SumScale;
	SumScale = processImageGray.ProcessImage(KeblerImage::topSobel);
	for (int x = 0; x < SumScale.m_height; x++)
	{
		for (int y = 0; y < SumScale.m_width; y++)
		{
			if (!processImageSobel.GetPixel(x, y).isBlack())
			{
				Color newColorLeft;
				Color neColorTop;
				newColorLeft = processImageSobel.GetPixel(x, y);
				neColorTop = SumScale.GetPixel(x, y);
				newColorLeft.r += neColorTop.r;
				newColorLeft.g += neColorTop.r;
				newColorLeft.b += neColorTop.r;
				if (newColorLeft.r > 255)
				{
					newColorLeft.r = 255;
					newColorLeft.g = 255;
					newColorLeft.b = 255;
				}
				SumScale.SetPixel(x, y, newColorLeft);
			}
		}
	}

	std::cout << "Sobel\n";

	///////////// Process Prewwit //////////////////////////////

	Image processImagePrewwit;
	processImagePrewwit = processImageGray.ProcessImage(KeblerImage::leftPrewwit);
	Image processImagePrewwitTop;
	processImagePrewwitTop = processImageGray.ProcessImage(KeblerImage::topPrewwit);

	std::cout << "Prewwit\n";

	////////////////// Address Texture ////////////////////////

	//Image imagebBitBlit;
	//imagebBitBlit.CreateImage(1920, 1080, imagenOriginal.m_bpp);
	//for (int iY = 0; iY < imagenOriginal.m_height; iY++)
	//{
	//	for (int iX = 0; iX < imagenOriginal.m_width; iX++)
	//	{
	//		float siU = (1.0f / imagenOriginal.m_width) * iX;
	//		float siV = (1.0f / imagenOriginal.m_height) * iY;
	//		Color color(imagenOriginal.SampleBilineal(siU, siV));
	//		bilinealImage.SetPixel(iX, iY, color);
	//	}
	//}
	
	auto codec = g_CodecMan.GetCodecByFileExt(".bmp");
	string nameFile = GetFileName(path);
	nameFile = "ImageProcess/" + nameFile;
	if (codec)
	{
		codec->Encode(imagenOriginal, nameFile + ".bmp");
		codec->Encode(bilinealImage, nameFile + "Bilineal.bmp");
		codec->Encode(imageScale, nameFile + "Scale.bmp");
		codec->Encode(imagebBitBlit, nameFile + "BitBlit.bmp");
		codec->Encode(imageRotate, nameFile + "Rotate.bmp");
		codec->Encode(imageRaster, nameFile + "Raster.bmp");
		codec->Encode(processImageGray, nameFile + "GrayScale.bmp");
		codec->Encode(processImageBlur, nameFile + "Blur.bmp");
		codec->Encode(processImageSobel, nameFile + "Sobel.bmp");
		codec->Encode(processImagePrewwit, nameFile + "leftPrewitt.bmp");
		codec->Encode(processImagePrewwitTop, nameFile + "PrewittTop.bmp");
		codec->Encode(SumScale, nameFile + "SobelSum.bmp");
	}

	std::cout << "Imagenes guardadas en la carpeta ImageProcess exitosamente. :)\nPresiona enter para salir.";
	std::cin.get();
	return 0;
}

