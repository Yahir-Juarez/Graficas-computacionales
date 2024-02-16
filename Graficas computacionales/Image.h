#pragma once

#include <cmath>
#include "AppTypes.h"
#include "FileUtil.h"
#include "ImgCodec.h"

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



	void SetPixel(uint32 x, uint32 y, Color color)
	{
		if (x < 0 || y < 0 || x >= m_width || y >= m_height) { return; }

		uint32 basePos = (y * m_pitch) + (x * (m_bpp >> 3));
		if (m_bpp == 24)
		{
			m_pixelData[basePos + 0] = static_cast<byte>(color.r);
			m_pixelData[basePos + 1] = static_cast<byte>(color.g);
			m_pixelData[basePos + 2] = static_cast<byte>(color.b);
		}
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

	int getRoationPosX(int x, int y, float grados)
	{
		float radianes = (grados * 3.1416) / 180;
		float posInX = (x * cos(radianes)) - (y * sin(radianes));
		return posInX;
	}
	int getRoationPosY(int x, int y, float grados)
	{
		float radianes = (grados * 3.1416) / 180;
		float posInY = (x * sin(radianes)) + (y * cos(radianes));
		return posInY;
	}

	void rotate(float grados, Image& imagen)
	{
		int poinX1 = getRoationPosX(0, 0, grados);
		int poinY1 = getRoationPosY(0, 0, grados);

		int poinX2 = getRoationPosX(imagen.m_width, 0, grados);
		int poinY2 = getRoationPosY(imagen.m_width, 0, grados);

		int poinX3 = getRoationPosX(imagen.m_width, imagen.m_height, grados);
		int poinY3 = getRoationPosY(imagen.m_width, imagen.m_height, grados);

		int poinX4 = getRoationPosX(0, imagen.m_height, grados);
		int poinY4 = getRoationPosY(0, imagen.m_height, grados);

		int maxX = max(max(max(poinX1, poinX2), poinX3), poinX4);
		int maxY = max(max(max(poinY1, poinY2), poinY3), poinY4);

		int minX = min(min(min(poinX1, poinX2), poinX3), poinX4);
		int minY = min(min(min(poinY1, poinY2), poinY3), poinY4);

		float ancho = abs(maxX) + abs(minX);
		float alto = abs(maxY) + abs(minY);

		CreateImage(ancho, alto, imagen.m_bpp);

		for (int x = 1; x < imagen.m_width; x++)
		{
			for (int y = 1; y < imagen.m_height; y++)
			{
				Color colorInBuffer = imagen.GetPixel(x, y);
				SetPixel((getRoationPosX(x, y, grados)) + (abs(minX) - 1), (getRoationPosY(x, y, grados)), colorInBuffer);
			}
		}

		//SetPixel((getRoationPosX(x, y, grados) + negativoX) - 1, (getRoationPosY(x, y, grados) + negativoY) - 1, colorInBuffer);
		for (int x = 0; x < m_width; x++)
		{
			for (int y = 0; y < m_height; y++)
			{
				Color colorInBuffer = GetPixel(x, y);
				if (colorInBuffer.isBlack())
				{
					colorInBuffer = GetPixel(x + 1, y);
					if (!colorInBuffer.isBlack())
					{
						colorInBuffer = GetPixel(x - 1, y);
						SetPixel(x, y, colorInBuffer);
					}
				}
			}
		}

		/*for (int y = 1; y < alto; y++)
		{
			SetPixel(2, y, Color(0, 0, 255));
			SetPixel(ancho - 1, y, Color(0, 0, 255));
		}
		for (int x = 1; x < ancho; x++)
		{
			SetPixel(x, 2, Color(0, 0, 255));
			SetPixel(x, alto - 2, Color(0, 0, 255));
		}*/
	}

	uint32 GetWidth()
	{
		return m_width;
	}
	uint32 GetHeight()
	{
		return m_height;
	}
	uint32 m_bpp = 0;
	uint32 m_width = 0;
	uint32 m_height = 0;
	uint32 m_pitch = 0;

	Vector<byte> m_pixelData;
};