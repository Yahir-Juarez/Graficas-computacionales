#include "myVector3.h"

void myVector3::Normalize()
{
	float lenght = sqrt(x * x + y * y + z * z);
	x /= lenght;
	y /= lenght;
	z /= lenght;
}
