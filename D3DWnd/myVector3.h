#pragma once

#include <cmath>

class myVector3
{
public:
	myVector3() = default;
	~myVector3() = default;

	myVector3(float pos_x, float pos_y, float pos_z) { x = pos_x; y = pos_y; z = pos_z; }

	void Normalize();

	float dot(const myVector3& rhs) const
	{
		return (x * rhs.x + y * rhs.y + z * rhs.z);
	}

	myVector3 operator-(const myVector3& rhs) const
	{
		myVector3 resultado;
		for (int i = 0; i < 3; ++i)
		{
			(&resultado.x)[i] = (&x)[i] - (&rhs.x)[i];
		}
		return myVector3(resultado);
	}

	myVector3 operator+(const myVector3& rhs) const
	{
		return myVector3(x + rhs.x, y + rhs.y, z + rhs.z);
	}

	myVector3 operator*(float scalar) const
	{
		return myVector3(x * scalar, y * scalar, z * scalar);
	}

	myVector3 operator/(float scalar) const
	{
		return myVector3(x / scalar, y / scalar, z / scalar);
	}

	myVector3 operator^(const myVector3& rhs) const 
	{
		myVector3 resultado;
		resultado.x = y * rhs.z - z * rhs.y;
		resultado.y = z * rhs.x - x * rhs.z;
		resultado.z = x * rhs.y - y * rhs.x;
		return myVector3(resultado);
	}

	float operator|(const myVector3& rhs) const 
	{
		float resultado = 0;
		for (int i = 0; i < 3; ++i) 
		{
			resultado += (&x)[i] * (&rhs.x)[i];
		}
		return resultado;
	}

	float x;
	float y;
	float z;
};