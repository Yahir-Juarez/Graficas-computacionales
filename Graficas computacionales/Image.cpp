#include "Image.h"
#include <iostream>

using namespace std;

void Image::rotate(float grados, Image& imagen)
{
	point1.pointX = getRoationPosX(0, 0, grados);
	point1.pointY = getRoationPosY(0, 0, grados);

	point2.pointX = getRoationPosX(imagen.m_width, 0, grados);
	point2.pointY = getRoationPosY(imagen.m_width, 0, grados);

	point4.pointX = getRoationPosX(imagen.m_width, imagen.m_height, grados);
	point4.pointY = getRoationPosY(imagen.m_width, imagen.m_height, grados);

	point3.pointX = getRoationPosX(0, imagen.m_height, grados);
	point3.pointY = getRoationPosY(0, imagen.m_height, grados);

	int maxX = max(max(max(point1.pointX, point2.pointX), point3.pointX), point4.pointX);
	int maxY = max(max(max(point1.pointY, point2.pointY), point3.pointY), point4.pointY);

	int minX = min(min(min(point1.pointX, point2.pointX), point3.pointX), point4.pointX);
	int minY = min(min(min(point1.pointY, point2.pointY), point3.pointY), point4.pointY);

	float ancho = abs(maxX) + abs(minX);
	float alto = abs(maxY) + abs(minY);
	
	point1.pointX += (abs(minX));
	point2.pointX += (abs(minX));
	point3.pointX += (abs(minX));
	point4.pointX += (abs(minX));

	point1.pointY += (abs(minY));
	point2.pointY += (abs(minY));
	point3.pointY += (abs(minY));
	point4.pointY += (abs(minY));

	CreateImage(ancho, alto, imagen.m_bpp);

	for (int x = 0; x < imagen.m_width; x++)
	{
		for (int y = 0; y < imagen.m_height; y++)
		{
			Color colorInBuffer = imagen.GetPixel(x, y);
			bool setPix  = SetPixel((getRoationPosX(x, y, grados)) + (abs(minX) - 1), (getRoationPosY(x, y, grados) + (abs(minY) - 1)), colorInBuffer);
			if (setPix == false)
			{
				SetPixel((getRoationPosX(x, y, grados)) + (abs(minX) - 1), (getRoationPosY(x, y, grados)), colorInBuffer);
			}
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

void Image::bitBlt(Image& src,
	int x,
	int y,
	int srcIniX,
	int srcIniY,
	int srcEndX,
	int srcEndY)
{
	if (x >= m_width || y >= m_height) 
	{ 
		return; 
	}
	if (0 == srcEndX) { srcEndX = src.m_width; }
	if (0 == srcEndY) { srcEndY = src.m_height; }
	if (x < 0)
	{
		srcIniX -= x;
		x = 0;
	}
	if (y < 0)
	{
		srcIniY -= y;
		y = 0;
	}

	int realWidth = srcEndX - srcIniX;
	int realHeight = srcEndY - srcIniY;

	if (realWidth < 0 || realHeight < 0)
	{
		return;
	}

	if ((x + realWidth) > m_width)
	{
		realWidth -= (x * realWidth) - m_width;
	}

	if ((y + realHeight) > m_height)
	{
		realHeight -= (y + realHeight) - m_height;
	}
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

	*/

	char* DestBuffer = reinterpret_cast<char*>(m_pixelData.data()) + (((y * m_width) + x) * (m_bpp / 8));
	char* SrcBuffer = reinterpret_cast<char*>(src.m_pixelData.data()) + (((srcIniY * src.m_width) + srcIniX) * (src.m_bpp / 8));

	int bytesToCopy = realWidth * (src.m_bpp / 8);

	/*for (int line = 0; line < src.m_height; ++line)
	{
		memcpy(DestBuffer, SrcBuffer, src.m_pitch);

		DestBuffer += m_pitch;
		SrcBuffer += src.m_pitch;
	}*/
	int pruebax = 0;
	for (int line = 0; line < realWidth; ++line)
	{
		memcpy(DestBuffer, SrcBuffer, bytesToCopy);

		DestBuffer += m_pitch;
		SrcBuffer += src.m_pitch;
		pruebax++;
	}
	bool i = true;
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

//vector2D Image::GetPointInLine(int x0, int y0, int x1, int y1, int& iter, bool& isValid)
//{
//	vector2D pointXY;
//	x0 += 1;
//	y1 += 1;
//	int dx = std::abs(x1 - x0);
//	int dy = std::abs(y1 - y0);
//
//	int x_inc = (x1 > x0) ? 1 : -1;
//	int y_inc = (y1 > y0) ? 1 : -1;
//
//	int error = dx - dy;
//
//	while (x0 != x1 || y0 != y1) {
//		std::cout << "(" << x0 << ", " << y0 << ")\n";
//		int error_2 = 2 * error;
//
//		if (error_2 > -dy) {
//			error -= dy;
//			x0 += x_inc;
//		}
//
//		if (error_2 < dx) {
//			pointXY.pointX = x0;
//			pointXY.pointY = y0;
//			return pointXY;
//			error += dx;
//			y0 += y_inc;
//		}
//	}
//	//vector2D pointXY;
//	//if (iter > 0)
//	//{
//	//	/*iter += 2 * (y0)+(-2 * (x0));
//	//	x0 += 1;
//	//	y0 += 1;*/
//	//	isValid = false;
//	//	return vector2D(x0, y0);
//	//}
//	//else
//	//{
//	//	iter += 2 * (y0);
//	//	if (x0 > x1)
//	//	{
//	//		x0 += 1;
//	//	}
//	//	else
//	//	{
//	//		x0 -= 1;
//	//	}
//	//	isValid = true;
//	//	pointXY = GetPointInLine(x0, y0, x1, y1, iter, isValid);
//	//}
//	return pointXY;
//}

vector2D Image::GetPointInLine(int x0, int y0, int x1, int y1, int& dx, int& dy, int& error)
{
	vector2D pointXY;
	/*x0 += 1;
	y1 += 1;*/
	

	int x_inc = (x1 > x0) ? 1 : -1;
	int y_inc = (y1 > y0) ? 1 : -1;

	while (x0 != x1 || y0 != y1) {
		int error_2 = 2 * error;

		if (error_2 > -dy) {
			error -= dy;
			x0 += x_inc;
		}

		if (error_2 < dx) {
			pointXY.pointX = (x0);
			pointXY.pointY = y0;
			error += dx;
			return pointXY;
			
			y0 += y_inc;
		}
	}
	pointXY.pointX = (x0);
	pointXY.pointY = y0;
	return pointXY;
}

void intercambiar(vector2D& a, vector2D& b) {
	vector2D temp = a;
	a = b;
	b = temp;
}

void bubbleSort(vector2D arreglo[], int tamano) {
	for (int i = 0; i < tamano - 1; ++i) {
		for (int j = 0; j < tamano - i - 1; ++j) {
			// Compara elementos adyacentes y los intercambia si están en el orden incorrecto
			if (arreglo[j].pointY > arreglo[j + 1].pointY) {
				intercambiar(arreglo[j], arreglo[j + 1]);
			}
		}
	}
}

Image Image::ProcessImage(std::function<Color(const Image&, int, int, const float[])> procFunction, const float matrix[])
{
	Image rt;
	rt.CreateImage(m_width, m_height, m_bpp);

	for (int iY = 0; iY < m_height; iY++)
	{
		for (int iX = 0; iX < m_width; iX++)
		{
			Color color = procFunction(*this, iX, iY, matrix);
			rt.SetPixel(iX, iY, color);
		}
	}
	return rt;
}

Image Image::ProcessImage(std::function<Color(const Image&, int, int)> procFunction)
{
	Image rt;
	rt.CreateImage(m_width, m_height, m_bpp);

	for (int iY = 0; iY < m_height; iY++)
	{
		for (int iX = 0; iX < m_width; iX++)
		{
			Color color = procFunction(*this, iX, iY);
			rt.SetPixel(iX, iY, color);
		}
	}
	return rt;
}


void Image::Raster(Image& src,
	int x,
	int y)
{
	vector2D arrayPoints[4] = { vector2D(src.getPoint1().pointX, src.getPoint1().pointY), vector2D(src.getPoint2().pointX, src.getPoint2().pointY), 
		vector2D(src.getPoint3().pointX, src.getPoint3().pointY), vector2D(src.getPoint4().pointX, src.getPoint4().pointY) };

	bubbleSort(arrayPoints, 4);
	if (arrayPoints[2].pointX > 1)
	{
		intercambiar(arrayPoints[2], arrayPoints[1]);
	}

	SetPixel(arrayPoints[0].pointX, arrayPoints[0].pointY, GetPixel(arrayPoints[0].pointX, arrayPoints[0].pointY));
	vector2D PointIterL(arrayPoints[0].pointX + 1, arrayPoints[0].pointY);
	vector2D PointIterR(arrayPoints[0].pointX + 1, arrayPoints[0].pointY);

	int dx = std::abs(arrayPoints[2].pointX - (PointIterL.pointX));
	int dy = std::abs((arrayPoints[2].pointY + 1)- arrayPoints[0].pointY);
	int error = dx - dy;

	int dx2 = std::abs(arrayPoints[1].pointX - (PointIterR.pointX));
	int dy2 = std::abs((arrayPoints[1].pointY + 1) - PointIterR.pointY);
	int error2 = dx2 - dy2;

	for (int fy = 0; fy < arrayPoints[3].pointY; ++fy)
	{
		if (fy == 78)
		{
			cout << "si";
		}
		cout << "Detector de puntos en linea hacia la izquierda\n";
		PointIterL = GetPointInLine((PointIterL.pointX), fy, arrayPoints[2].pointX, (arrayPoints[2].pointY + 1), dx, dy, error);
		cout << "Fin de recorrimiento\n\n";
		PointIterR = GetPointInLine((PointIterR.pointX), fy, arrayPoints[1].pointX, (arrayPoints[1].pointY + 1), dx2, dy2, error2);
		for (int fx = PointIterL.pointX; fx <= PointIterR.pointX; fx++)
		{
			/////////////////////////////////
			//No dejar esto			/////
			//isBlack() muy racista	/////
			//como para estar en el	/////
			//codigo xD				/////
			/////////////////////////////////
			if (!(src.GetPixel(fx, fy)).isBlack())
			{
				
			}
			SetPixel((fx + x), (fy + y), src.GetPixel(fx, fy));
		}
		if (PointIterL.pointX == (arrayPoints[2].pointX) && fy == (arrayPoints[2].pointY + 1))
		{
			PointIterL.pointX = arrayPoints[2].pointX + 1;
			PointIterL.pointY = arrayPoints[2].pointY + 1;
			arrayPoints[2].pointX = arrayPoints[3].pointX;
			arrayPoints[2].pointY = arrayPoints[3].pointY;
			dx = std::abs(arrayPoints[2].pointX - (PointIterL.pointX));
			dy = std::abs((arrayPoints[2].pointY + 1)- PointIterL.pointY);
			error = dx - dy;
		}
		if (PointIterR.pointX == (arrayPoints[1].pointX) && fy == (arrayPoints[1].pointY + 1))
		{
			PointIterR.pointX = arrayPoints[1].pointX + 1;
			PointIterR.pointY = arrayPoints[1].pointY + 1;
			arrayPoints[1].pointX = arrayPoints[3].pointX;
			arrayPoints[1].pointY = arrayPoints[3].pointY;
			dx2 = std::abs(arrayPoints[1].pointX - (PointIterR.pointX));
			dy2 = std::abs((arrayPoints[1].pointY + 1)- PointIterR.pointY);
			error2 = dx2 - dy2;
		}
	}
}


void Image::DrawLine(int x0, int y0, int x1, int y1, Color color)
{
	int dx, dy, p, x, y;

	if (x0 > x1) { swap(x0, x1); }
	if (y0 > y1) { swap(y0, y1); }

	dx = x1 - x0;
	dy = y1 - y0;

	if (dy == 0)
	{
		int startPos = m_pitch + y0 + (x0 + m_bpp);
		if (m_bpp == 4)
		{
			int memlenght = dx * m_bpp;
			char tmpColor[4];
			tmpColor[2] = color.r;
			tmpColor[1] = color.g;
			tmpColor[0] = color.b;
			//tmpColor[3] = color.;

			memset((int*)m_pixelData[startPos], *(int*)tmpColor, memlenght);
			return;
		}
		else
		{
			for (int i = 0; i < dx; ++i)
			{
				m_pixelData[startPos + 0] = static_cast<byte>(color.b);
				m_pixelData[startPos + 1] = static_cast<byte>(color.g);
				m_pixelData[startPos + 2] = static_cast<byte>(color.r);
				startPos += m_bpp;
			}
			return;
		}
	}
	else if (dx == 0)
	{
		for (int i = y0; i <= y1; ++i)
		{
			SetPixel(x0, i, color);
		}
	}

	x = x0;
	y = y0;

	p = 2 * dy - dx;

	while (x < x1)
	{
		SetPixel(x, y, color);
		if (p >= 0)
		{
			y = y + 1;
			p = p + 2 * dy - 2 * dx;
		}
		else
		{
			p = p + 2 * dy;
		}
		x = x + 1;
	}
}

void Image::draw_line(int x0, int y0, int x1, int y1) {
	x0 += 1;
	y1 += 1;
	int dx = std::abs(x1 - x0);
	int dy = std::abs(y1 - y0);

	int x_inc = (x1 > x0) ? 1 : -1;
	int y_inc = (y1 > y0) ? 1 : -1;

	int error = dx - dy;

	while (x0 != x1 || y0 != y1) {
		std::cout << "(" << x0 << ", " << y0 << ")\n";
		SetPixel((x0 + 50), (y0 + 50), Color(0, 0, 255));
		int error_2 = 2 * error;

		if (error_2 > -dy) {
			error -= dy;
			x0 += x_inc;
		}

		if (error_2 < dx) {
			error += dx;
			y0 += y_inc;
		}
	}
}






/*
void Image::rotate(float grados, Image& imagen)
{
	point1.pointX = getRoationPosX(0, 0, grados);
	point1.pointY = getRoationPosY(0, 0, grados);

	point2.pointX = getRoationPosX(imagen.m_width, 0, grados);
	point2.pointY = getRoationPosY(imagen.m_width, 0, grados);

	point4.pointX = getRoationPosX(imagen.m_width, imagen.m_height, grados);
	point4.pointY = getRoationPosY(imagen.m_width, imagen.m_height, grados);

	point3.pointX = getRoationPosX(0, imagen.m_height, grados);
	point3.pointY = getRoationPosY(0, imagen.m_height, grados);

	int maxX = max(max(max(point1.pointX, point2.pointX), point3.pointX), point4.pointX);
	int maxY = max(max(max(point1.pointY, point2.pointY), point3.pointY), point4.pointY);

	int minX = min(min(min(point1.pointX, point2.pointX), point3.pointX), point4.pointX);
	int minY = min(min(min(point1.pointY, point2.pointY), point3.pointY), point4.pointY);

	float ancho = abs(maxX) + abs(minX);
	float alto = abs(maxY) + abs(minY);

	point1.pointX += (abs(minX)) + 1;
	point2.pointX += (abs(minX)) + 1;
	point3.pointX += (abs(minX)) + 1;
	point4.pointX += (abs(minX)) + 1;

	CreateImage(ancho, alto, imagen.m_bpp);

	for (int x = 1; x <= imagen.m_width; x++)
	{
		for (int y = 1; y <= imagen.m_height; y++)
		{
			Color colorInBuffer = imagen.GetPixel(x, y);
			bool setPix = SetPixel((getRoationPosX(x, y, grados)) + (abs(minX)), (getRoationPosY(x, y, grados)), colorInBuffer);
			if (setPix == false)
			{
				SetPixel((getRoationPosX(x, y, grados)) + (abs(minX) - 1), (getRoationPosY(x, y, grados)), colorInBuffer);
			}
		}
	}
}
*/