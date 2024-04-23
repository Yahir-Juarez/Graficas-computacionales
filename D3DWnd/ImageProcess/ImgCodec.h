#pragma once

#include <fstream>
//#include <Windows.h>
//#include <wingdi.h>

#include "AppTypes.h"
#include "FileUtil.h"

class Image;


using namespace std;

class ImgCodec
{
public:
	ImgCodec() = default;
	virtual ~ImgCodec() = default;

	virtual bool CanDecodeExt(const String& ext) = 0;

	virtual bool Decode(const Path& filePath) = 0;
	virtual bool Encode(const Image& image, const String& fileName) = 0;
	virtual void Clear() = 0;

	virtual uint32 GetBitsPerPixel() 
	{
		return m_bpp;
	}
	virtual uint32 GetImageWidth() 
	{
		return m_width;
	}
	virtual uint32 GetImageHeight() 
	{
		return m_heihgt;
	}
	virtual Vector<Byte> GetPixelData() 
	{
		return m_pixelData;
	}
	
	virtual bool IsReady()
	{
		return m_isReady;
	}
	

protected:
	bool m_isReady = false;

	uint32 m_bpp = 0;
	uint32 m_width = 0;
	uint32 m_heihgt = 0;
	Vector<Byte> m_pixelData;
};

class BMPCodec : public ImgCodec
{
public:
	BMPCodec() = default;
	virtual ~BMPCodec() = default;

	bool CanDecodeExt(const String& ext) override
	{
		static String codecExt(".bmp");
		String lExt(ext);
		FileUtil::toLowerCase(lExt);
		return codecExt.compare(lExt) == 0;
	}

	bool Decode(const Path& filePath) override;

	bool Encode(const Image& image, const String& fileName) override;

	void Clear() override
	{
		m_isReady = false;
		m_bpp = 0;
		m_width = 0;
		m_heihgt = 0;
		m_pixelData.clear();
	}
};

class CodecMnager
{
public:
	CodecMnager() = default;
	~CodecMnager()
	{
		for (auto pCodec : m_codec)
		{
			delete pCodec;
		}
	}

	void AddCodec(ImgCodec* pCodec)
	{
		m_codec.push_back(pCodec);
	}

	ImgCodec* GetCodecByFileExt(const String& ext)
	{
		for (auto pCodec : m_codec)
		{
			if (pCodec->CanDecodeExt(ext))
			{
				return pCodec;
			}
		}

		return nullptr;
	}
private:
	Vector<ImgCodec*> m_codec;
};

static CodecMnager g_CodecMan;