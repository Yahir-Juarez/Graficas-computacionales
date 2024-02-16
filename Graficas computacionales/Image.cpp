#include "Image.h"

void Image::bitBlt(Image& src,
	int x,
	int y,
	int srcIniX,
	int srcIniY,
	int srcEndX,
	int srcEndY)
{
	/*if (0 == srcEndX)
	{
		srcEndX = src.GetWidth();
	}
	if (0 == srcEndY)
	{
		srcEndY = src.GetHeight();
	}

	int realWidth = srcEndX - srcIniX;
	int realHeight = srcEndY - srcIniY;

	if (x < 0)
	{
		srcIniX -= x;
		x = 0;
	}*/

	char* DestBuffer = reinterpret_cast<char*>(m_pixelData.data()) + (x * m_bpp);
	char* SrcBuffer = reinterpret_cast<char*>(src.m_pixelData.data());

	for (int line = 0; line < src.m_height; ++line)
	{
		memcpy(DestBuffer, SrcBuffer, src.m_pitch);

		DestBuffer += m_pitch;
		SrcBuffer += src.m_pitch;
	}

	/*bool i;
	i = false;
	char* DestBuffer = reinterpret_cast<char*>(m_pixelData.data() + (((y * m_width) + x) * m_bpp));
	char* SrcBuffer = reinterpret_cast<char*>(src.m_pixelData.data()) + (((srcIniY * src.GetWidth()) + srcIniX) * src.m_bpp);

	int bytesToCopy = (src.GetWidth() - srcIniX) * src.m_bpp;

	for (int line = srcIniY; line < srcEndY; ++line)
	{
		memcpy(DestBuffer, SrcBuffer, bytesToCopy);

		DestBuffer += m_pitch;
		SrcBuffer += src.m_pitch;
	}*/

	/*int sx = srcEndX - srcIniX; sx = sx == 0 ? src.GetWidth() - 1 : sx;
	int sy = srcEndY - srcIniY; sy = sy == 0 ? src.GetHeight() - 1 : sy;

	if (x < 0) { srcIniX += abs(x); x = 0; }
	if (y < 0) { srcIniY += abs(y); y = 0; }
	if (y > m_width || y >= m_height) { return; }*/

}