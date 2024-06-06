#pragma once

class myVector3
{
public:
	myVector3() = default;
	~myVector3() = default;

	myVector3(float pos_x, float pos_y, float pos_z) { x = pos_x; y = pos_y; z = pos_z; }

	myVector3 getVectorNormalize();

	myVector3 operator-(const myVector3& otro) const {
		myVector3 resultado;
		for (int i = 0; i < 3; ++i) {
			(&resultado.x)[i] = (&x)[i] - (&otro.x)[i];
		}
		return myVector3(resultado);
	}

	myVector3 operator^(const myVector3& otro) const {
		myVector3 resultado;
		resultado.x = y * otro.z - z * otro.y;
		resultado.y = z * otro.x - x * otro.z;
		resultado.z = x * otro.y - y * otro.x;
		return myVector3(resultado);
	}

	float operator|(const myVector3& otro) const {
		int resultado = 0;
		for (int i = 0; i < 3; ++i) {
			resultado += (&x)[i] * (&otro.x)[i];
		}
		return resultado;
	}
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
};