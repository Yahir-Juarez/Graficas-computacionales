#pragma once

#include <cmath>
#include "AppTypes.h"
#include "FileUtil.h"
#include "ImgCodec.h"
#include <iostream>
#include <functional>

using std::cout;

using std::max;
using std::min;

class Color
{
public:
	Color() = default;
	Color(uint8 _r, uint8 _g, uint8 _b)
		: r(_r), g(_g), b(_b)
	{}

	bool isBlack()
	{
		if (r == 0)
		{
			if (g == 0)
			{
				if (b == 0)
				{
					return true;
				}
			}
		}
		return false;
	}
	~Color() = default;

	unsigned char r;
	unsigned char g;
	unsigned char b;
};

struct Rect
{
	int left = 0;
	int top = 0;
	int right = 0;
	int bottom = 0;

	int GetWidth() { right - left; }
	int GetHeight() { bottom - top; }
};

struct vector2D
{
	vector2D() 
	{
		pointX = 0;
		pointY = 0;
	}
	vector2D(float x, float y)
	{
		pointX = x;
		pointY = y;
	}
	float pointX;
	float pointY;
};

class Image
{
public:
	Image() = default;
	~Image() = default;

	//Al normalizar la direccion de un vector el conjunto de X y Y nos da 1 Explicacion 9:36 31/01/2024

	void CreateImage(int32 width, uint32 height, uint32 bpp)
	{
		m_pixelData.clear();

		m_bpp = bpp;
		m_width = width;
		m_height = height;
		m_pitch = m_width * static_cast<int32>(std::ceilf(bpp / 8.0f));

		int32 numBytes = static_cast<int32>(std::ceilf(float(width) *
														float(height) *
														(float(bpp) / 8.0f)));
		m_pixelData.resize(numBytes);
	}



	bool SetPixel(uint32 x, uint32 y, Color color)
	{
		if (x < 0 || y < 0 || x >= m_width || y >= m_height) { return false; }

		//cout << "Pixel X -> " << x << " Y -> " << y << '\n';

		uint32 basePos = (y * m_pitch) + (x * (m_bpp >> 3));
		if (m_bpp == 24)
		{
			m_pixelData[basePos + 0] = static_cast<byte>(color.r);
			m_pixelData[basePos + 1] = static_cast<byte>(color.g);
			m_pixelData[basePos + 2] = static_cast<byte>(color.b);
		}
		return true;
	}

	Color GetPixel(uint32 x, uint32 y) const
	{
		if (x < 0 || y < 0 || x >= m_width || y >= m_height) { return Color(); }

		uint32 basePos = (y * m_pitch) + (x * (m_bpp >> 3));
		if (m_bpp == 24)
		{
			return Color(static_cast<uint8>(m_pixelData[basePos + 0]),
						static_cast<uint8>(m_pixelData[basePos + 1]),
						static_cast<uint8>(m_pixelData[basePos + 2]));
		}

		return Color();
	}

	Color SamplePixel(float u, float v) const
	{
		uint32 pixX = m_width * u;
		uint32 pixY = m_height * v;

		return GetPixel(pixX, pixY);
	}

	void CreateFromImageFile(const Path& filePath)
	{
		String fileExt = FileUtil::GetFileExtension(filePath);

		ImgCodec* pCodec = g_CodecMan.GetCodecByFileExt(fileExt);

		if (!pCodec)
		{
			pCodec->Clear();
			return;
		}

		if (!pCodec->Decode(filePath))
		{
			pCodec->Clear();
			return;
		}

		m_bpp = pCodec->GetBitsPerPixel();
		m_width = pCodec->GetImageWidth();
		m_height = pCodec->GetImageHeight();
		m_pitch = m_width * static_cast<int32>(std::ceilf(m_bpp / 8.0f));

		m_pixelData.clear();
		m_pixelData = pCodec->GetPixelData();
	}

	void bitBlt(Image& src,
		int x = 0,
		int y = 0,
		int srcIniX = 0,
		int srcIniY = 0,
		int srcEndX = 0,
		int srcEndY = 0);

	void bitBltImgRotate(Image& src,
		int x = 0,
		int y = 0);

	Image ProcessImage(std::function<Color(const Image&, int, int, const float[])> procFunction, const float matrix[] = NULL);
	Image ProcessImage2(Color *procFunction(const Image&, int, int));

	bool scaleImg(float scale, Image& ptrImg)
	{
		CreateImage(ptrImg.m_width * scale, ptrImg.m_height * scale,
			ptrImg.m_bpp);

		float siU = 1.0f / m_width;
		float siV = 1.0f / m_height;

		for (int y = 0; y < m_height; ++y)
		{
			for (int x = 0; x < m_width; x++)
			{
				SetPixel(x, y, ptrImg.SamplePixel(siU * x,
					siV * y));
			}
		}
		return true;
	}

	void draw_line(int x0, int y0, int x1, int y1);

	int getRoationPosX(int x, int y, float grados)
	{
		float radianes = (grados * 3.1416) / 180;
		float posInX = (x * cos(radianes)) - (y * sin(radianes));
		double redondeado = round(posInX);

		if (posInX - floor(posInX) >= 0.5) {
			redondeado = ceil(posInX);
		}
		else {
			redondeado = floor(posInX);
		}
		return posInX;
	}
	int getRoationPosY(int x, int y, float grados)
	{
		float radianes = (grados * 3.1416) / 180;
		float posInY = (x * sin(radianes)) + (y * cos(radianes));
		double redondeado = round(posInY);

		if (posInY - floor(posInY) >= 0.5) {
			redondeado = ceil(posInY);
		}
		else {
			redondeado = floor(posInY);
		}

		return posInY;
	}

	void rotate(float grados, Image& imagen);

	//vector2D GetPointInLine(int x0, int y0, int x1, int y1, int &iter, bool& isValid);
	vector2D GetPointInLine(int x0, int y0, int x1, int y1, int& dx, int& dy, int& error);

	void DrawLine(int x0, int y0, int x1, int y1, Color color);

	uint32 GetWidth()
	{
		return m_width;
	}
	uint32 GetHeight()
	{
		return m_height;
	}

	vector2D getPoint1()
	{
		return point1;
	}
	vector2D getPoint2()
	{
		return point2;
	}
	vector2D getPoint3()
	{
		return point3;
	}
	vector2D getPoint4()
	{
		return point4;
	}

	uint32 m_bpp = 0;
	uint32 m_width = 0;
	uint32 m_height = 0;
	uint32 m_pitch = 0;

	Vector<byte> m_pixelData;

	vector2D point1;
	vector2D point2;
	vector2D point3;
	vector2D point4;
};

//bool Scale(float scale)
	//{
	//	m_pitch *= scale;
	//	m_width *= scale;
	//	m_height *= scale;

	//	Vector<byte> scalePixels;
	//	scalePixels.resize(m_pitch * m_height);
	//	int inByte = 0;
	//	int inByteNew = 0;
	//	//cout << '\n';
	//	for (int i = 0; i < m_pixelData.size(); i++)
	//	{
	//		if (scale)
	//		{
	//			for (int j = 0; j < scale; j++)
	//			{
	//				//cout << inByte;
	//				scalePixels[inByte] = (m_pixelData[i + 0]);
	//				inByte += 1;
	//				//cout << ' ' << inByte;
	//				scalePixels[inByte] = (m_pixelData[i + 1]);
	//				inByte += 1;
	//				//cout << ' ' << inByte << '\n';
	//				scalePixels[inByte] = (m_pixelData[i + 2]);
	//				inByte += 1;
	//				inByteNew += 3;
	//			}
	//			i += 2;
	//			if (inByteNew >= m_pitch)
	//			{
	//				for (int SC = 1; SC < scale; ++SC)
	//				{
	//					memcpy(scalePixels.data() + (inByte), scalePixels.data() + inByte - m_pitch, m_pitch);
	//					inByte += inByteNew;
	//				}
	//				inByteNew = 0;
	//			}
	//			//scale = .5;
	//		}
	//		else
	//		{
	//			scale += scale;
	//		}
	//	}
	//	m_pixelData = scalePixels;
	//	return true;
	//}