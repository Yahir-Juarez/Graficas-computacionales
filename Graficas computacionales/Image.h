#pragma once

#include <cmath>
#include "AppTypes.h"
#include "FileUtil.h"
#include "ImgCodec.h"
#include <iostream>

using std::cout;

class Color
{
public:
	Color() = default;
	Color(uint8 _r, uint8 _g, uint8 _b)
		: r(_r), g(_g), b(_b)
	{}
	~Color() = default;

	unsigned char r;
	unsigned char g;
	unsigned char b;
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

		cout << pCodec->GetImageHeight();
		m_bpp = pCodec->GetBitsPerPixel();
		m_width = pCodec->GetImageWidth();
		m_height = pCodec->GetImageHeight();
		m_pitch = m_width * static_cast<int32>(std::ceilf(m_bpp / 8.0f));

		m_pixelData.clear();
		m_pixelData = pCodec->GetPixelData();
	}

	bool Scale(float scale)
	{
		m_pitch *= scale;
		m_width *= scale;
		m_height *= scale;

		Vector<byte> scalePixels;
		scalePixels.resize(m_pitch * m_height);
		int inByte = 0;
		int inByteNew = 0;
		//cout << '\n';
		for (int i = 0; i < m_pixelData.size(); i++)
		{
			if (scale)
			{
				for (int j = 0; j < scale; j++)
				{
					//cout << inByte;
					scalePixels[inByte] = (m_pixelData[i + 0]);
					inByte += 1;
					//cout << ' ' << inByte;
					scalePixels[inByte] = (m_pixelData[i + 1]);
					inByte += 1;
					//cout << ' ' << inByte << '\n';
					scalePixels[inByte] = (m_pixelData[i + 2]);
					inByte += 1;
					inByteNew += 3;
				}
				i += 2;
				if (inByteNew >= m_pitch)
				{
					for (int SC = 1; SC < scale; ++SC)
					{
						memcpy(scalePixels.data() + (inByte), scalePixels.data() + inByte - m_pitch, m_pitch);
						inByte += inByteNew;
					}
					inByteNew = 0;
				}
				//scale = .5;
			}
			else
			{
				scale += scale;
			}
		}
		m_pixelData = scalePixels;
		return true;
	}

	bool scaleImg2(float scale, Image& ptrImg)
	{
		CreateImage(m_width * scale, m_height * scale, m_bpp);

		float siU = 1.0f / m_width;
		float siV = 1.0f / m_height;

		for (int y = 0; y < m_height; ++y)
		{
			for (int x = 0; x < m_width; x++)
			{
				SetPixel(x, y, ptrImg.SamplePixel(siU * x, siV * y));
			}
		}
		return true;
	}

	uint32 m_bpp = 0;
	uint32 m_width = 0;
	uint32 m_height = 0;
	uint32 m_pitch = 0;

	Vector<byte> m_pixelData;
};