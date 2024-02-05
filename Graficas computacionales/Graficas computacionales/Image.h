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

	Color GetPixel(uint32 x, uint32 y)
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

	uint32 m_bpp = 0;
	uint32 m_width = 0;
	uint32 m_height = 0;
	uint32 m_pitch = 0;

	Vector<byte> m_pixelData;
};