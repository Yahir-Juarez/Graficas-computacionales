#include "myVector3.h"

myVector3 myVector3::getVectorNormalize()
{
	myVector3 newValor;
	if (x == 0.0f)
	{
		newValor.x = x;
	}
	else if (x < 0.0f)
	{
		newValor.x = x / (-1 * x);
	}
	else
	{
		newValor.x = x / x;
	}
	if (y == 0.0f)
	{
		newValor.y = y;
	}
	else if (y < 0.0f)
	{
		newValor.y = y / (-1 * y);
	}
	else
	{
		newValor.y = y / y;
	}
	if (z == 0.0f)
	{
		newValor.z = z;
	}
	else if (z < 0.0f)
	{
		newValor.z = z / (-1 * z);
	}
	else
	{
		newValor.z = z / z;
	}
	return newValor;
}
