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

#include <iostream>

class myVector4 {
public:
	float x, y, z, w;

	// Constructor por defecto - inicializa todos los valores en 0
	myVector4() : x(0), y(0), z(0), w(0) {}

	// Constructor con parámetros para inicializar todos los componentes
	myVector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

	// Constructor que inicializa todos los componentes con un mismo valor
	myVector4(float value) : x(value), y(value), z(value), w(value) {}

	// Sobrecarga del operador de suma
	myVector4 operator+(const myVector4& other) const {
		return myVector4(x + other.x, y + other.y, z + other.z, w + other.w);
	}

	// Sobrecarga del operador de resta
	myVector4 operator-(const myVector4& other) const {
		return myVector4(x - other.x, y - other.y, z - other.z, w - other.w);
	}

	// Sobrecarga del operador de multiplicación por un escalar
	myVector4 operator*(float scalar) const {
		return myVector4(x * scalar, y * scalar, z * scalar, w * scalar);
	}

	// Sobrecarga del operador de asignación
	myVector4& operator=(const myVector4& other) {
		if (this == &other)
			return *this; // Evita la autoasignación

		x = other.x;
		y = other.y;
		z = other.z;
		w = other.w;

		return *this;
	}
};


struct float3
{
	float x, y, z;
};
struct Light
{
	enum LIGHT_TYPE : unsigned int
	{
		DIRECTIONA = 0,
		POINT,
		SPOT

	};
	unsigned int Type;
	float SpotCutoff;
	float SpotExponent;
	myVector3 lightDir;
	myVector3 LightPos;
	myVector3 LightColor;
};
