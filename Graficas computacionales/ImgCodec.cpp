#include "ImgCodec.h"
#include "Image.h"
#include <iostream>
#include <cassert>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

using std::cout;

//pitch: cantidad de bytes necesarios para representar una fila completa de píxeles en la imagen

bool BMPCodec::Decode(const Path& filePath)
{
	fstream imgFile(filePath.c_str(), ios::in | ios::binary | ios::ate);
	if (!imgFile.is_open())
	{
		std::cout << "No abrio el archivo" << endl;
		return false;
	}

	auto fileSize = imgFile.tellg();
	imgFile.seekp(ios::beg);
	cout << "hola";
	BITMAPFILEHEADER bmpFH;
	imgFile.read(reinterpret_cast<char*>(&bmpFH), sizeof(bmpFH));

	/*Vector<char> fileData;
	fileData.resize(fileSize);*/


	//memcpy(&bmpFH, fileData.data(), sizeof(bmpFH));

	if (bmpFH.bfType != 0x4d42) //No es un bitmap
	{
		return false;
	}

	BITMAPINFOHEADER bmpInfo;
	imgFile.read(reinterpret_cast<char*>(&bmpInfo), sizeof(bmpInfo));

	if (bmpInfo.biBitCount < 24)
	{
		return false;
	}

	m_bpp = bmpInfo.biBitCount;
	m_width = bmpInfo.biWidth;
	m_heihgt = bmpInfo.biHeight;

	imgFile.seekp(bmpFH.bfOffBits);

	int32 numBytes = static_cast<int32>(std::ceilf(float(m_width) *
		float(m_heihgt) *
		(float(m_bpp) / 8.0f)));

	m_pixelData.resize(numBytes);

	int pitch = m_width * static_cast<int32>(std::ceilf(m_bpp / 8.0f));
	int padding = pitch % 4;
	int lineMemoryWidth = pitch + padding;

	for (int line = m_heihgt - 1; line >= 0; --line)
	{
		imgFile.seekp(lineMemoryWidth * line + bmpFH.bfOffBits);
		imgFile.read(reinterpret_cast<char*>(&m_pixelData[pitch * (m_heihgt - 1 - line)]), pitch);
	}
	m_pixelData;
	m_isReady = true;

	return m_isReady;
}

bool BMPCodec::Encode(const Image& image, const String& fileName)
{
	fstream imgFile(fileName.c_str(), ios::out | ios::binary | ios::beg);

	if (!imgFile.is_open())
	{
		return false;
	}

	int padding = image.m_pitch % 4;
	int lineMemoryWidth = image.m_pitch;
	if(padding)
	{
		padding = 4 - padding;
		lineMemoryWidth += padding;
	}
	assert(lineMemoryWidth % 4 == 0);
	if (true);
	BITMAPFILEHEADER bmpFH;
	memset(&bmpFH, 0, sizeof(bmpFH));
	bmpFH.bfType = 0x4d42;
	bmpFH.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + (lineMemoryWidth * image.m_height);
	bmpFH.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	BITMAPINFOHEADER bmpInfo;
	memset(&bmpInfo, 0, sizeof(bmpInfo));

	bmpInfo.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfo.biWidth = image.m_width;
	bmpInfo.biHeight = image.m_height;
	bmpInfo.biPlanes = 1;
	bmpInfo.biBitCount = image.m_bpp;
	bmpInfo.biCompression = BI_RGB;
	bmpInfo.biXPelsPerMeter = 3780;
	bmpInfo.biYPelsPerMeter = 3780;

	imgFile.write(reinterpret_cast<const char*>(&bmpFH), sizeof(bmpFH));
	imgFile.write(reinterpret_cast<const char*>(&bmpInfo), sizeof(bmpInfo));

	Vector<char> paddingBuffer;
	paddingBuffer.resize(3);

	for (int line = image.m_height - 1; line >= 0; --line)
	{
		imgFile.write(reinterpret_cast<const char*>(&image.m_pixelData[image.m_pitch * line]), image.m_pitch);

		if (padding)
		{
			imgFile.write(paddingBuffer.data(), padding);
		}
	}
	return 0;
}



//bool BMPCodec::EncodeScale(const Image& image, const String& fileName, int size)
//{
//	uint32 scalePitch = image.m_pitch * size;
//	uint32 scaleWidth = image.m_width * size;
//	uint32 scaleHeight = image.m_height * size;
//
//	fstream imgFile(fileName.c_str(), ios::out | ios::binary | ios::beg);
//
//	if (!imgFile.is_open())
//	{
//		return false;
//	}
//
//	int padding = scalePitch % 4;
//	int lineMemoryWidth = scalePitch + padding;
//
//	BITMAPFILEHEADER bmpFH;
//	memset(&bmpFH, 0, sizeof(bmpFH));
//	bmpFH.bfType = 0x4d42;
//	bmpFH.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + (lineMemoryWidth * image.m_height);
//	bmpFH.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
//
//	BITMAPINFOHEADER bmpInfo;
//	memset(&bmpInfo, 0, sizeof(bmpInfo));
//
//	bmpInfo.biSize = sizeof(BITMAPINFOHEADER);
//	bmpInfo.biWidth = scaleWidth;
//	bmpInfo.biHeight = scaleHeight;
//	bmpInfo.biPlanes = 1;
//	bmpInfo.biBitCount = image.m_bpp;
//	bmpInfo.biCompression = BI_RGB;
//	bmpInfo.biXPelsPerMeter = 3780;
//	bmpInfo.biYPelsPerMeter = 3780;
//
//	imgFile.write(reinterpret_cast<const char*>(&bmpFH), sizeof(bmpFH));
//	imgFile.write(reinterpret_cast<const char*>(&bmpInfo), sizeof(bmpInfo));
//
//	Vector<char> paddingBuffer;
//	paddingBuffer.resize(3);
//
//	/*for (int line = image.m_height - 1; line >= 0; --line)
//	{
//		imgFile.write(reinterpret_cast<const char*>(&image.m_pixelData[image.m_pitch * line]), image.m_pitch);
//
//		if (padding)
//		{
//			imgFile.write(paddingBuffer.data(), padding);
//		}
//	}*
//	
//	//Un zoom raro
//
//	/*for (int line = image.m_height - 1; line >= 0; --line)
//	{
//		imgFile.write(reinterpret_cast<const char*>(&image.m_pixelData[image.m_pitch * line]), image.m_pitch);
//		imgFile.write(reinterpret_cast<const char*>(&image.m_pixelData[image.m_pitch * line]), image.m_pitch);
//		imgFile.write(reinterpret_cast<const char*>(&image.m_pixelData[image.m_pitch * line]), image.m_pitch);
//
//		if (padding)
//		{
//			imgFile.write(paddingBuffer.data(), padding);
//		}
//	}*/
//
//	//Reinterpret pixelData
//
//	Vector<byte> scalePixels;
//
//	for (int i = 0; i < image.m_pixelData.size(); i++)
//	{
//		for (int j = 0; j < size; j++)
//		{
//			scalePixels.push_back(image.m_pixelData[i + 0]);
//			scalePixels.push_back(image.m_pixelData[i + 1]);
//			scalePixels.push_back(image.m_pixelData[i + 2]);
//		}
//		i += 2;
//	}
//
//	for (int line = (image.m_height * size) - 1; line >= 0; --line)
//	{
//		imgFile.write(reinterpret_cast<const char*>(&scalePixels[scalePitch * line]), scalePitch);
//
//		if (padding)
//		{
//			imgFile.write(paddingBuffer.data(), padding);
//		}
//	}
//
//	/*for (int line = image.m_height - 1; line >= 0; --line)
//	{
//		for (int i = 0; i < m_width; ++i)
//		{
//			for (int scale = size; 3 < 0; --scale)
//			{
//				imgFile.write(reinterpret_cast<const char*>(&image.m_pixelData[(i + 0) * line]), 8);
//				imgFile.write(reinterpret_cast<const char*>(&image.m_pixelData[(i + 0) * line]), 8);
//				imgFile.write(reinterpret_cast<const char*>(&image.m_pixelData[(i + 0) * line]), 8);
//			}
//			i += 2;
//		}
//
//		if (padding)
//		{
//			imgFile.write(paddingBuffer.data(), padding);
//		}
//	}*/
//	return 0;
//}