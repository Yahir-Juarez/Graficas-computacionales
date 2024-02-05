#pragma once

#include "AppTypes.h"

#include <algorithm>

namespace FileUtil
{
	inline String GetFileExtension(const Path& fileName)
	{
		auto pos = fileName.find_first_of('.');
		if (String::npos == pos)
		{
			return String();
		}

		return fileName.substr(pos, fileName.length() - pos);
	}

	/*inline ImgCodec* GetCodecByFileExt(const String& fileExt)
	{
		return nullptr;
	}*/

	inline void toLowerCase(String& str)
	{
		std::transform(str.begin(), str.end(), str.begin(), 
			[](char c) 
			{
			return static_cast<char>(::tolower(c));
			}
		);
	}
}