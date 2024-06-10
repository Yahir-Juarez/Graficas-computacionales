#pragma once

#include "myVector3.h"
#include "Prerequisitos.h"

template <typename T>
class myMatrix
{
public:
	myMatrix()
	{
		for (int y = 0; y < 4; y++)
		{
			for (int x = 0; x < 4; x++)
			{
				m_matrix[y][x] = 0;
			}
		}
	}
	myMatrix(float _m00, float _m01, float _m02, float _m03,
		float _m10, float _m11, float _m12, float _m13,
		float _m20, float _m21, float _m22, float _m23,
		float _m30, float _m31, float _m32, float _m33)
	{
		m_matrix[0][0] = _m00; m_matrix[0][1] = _m01; m_matrix[0][2] = _m02; m_matrix[0][3] = _m03;
		m_matrix[1][0] = _m10; m_matrix[1][1] = _m11; m_matrix[1][2] = _m12; m_matrix[1][3] = _m13;
		m_matrix[2][0] = _m20; m_matrix[2][1] = _m21; m_matrix[2][2] = _m22; m_matrix[2][3] = _m23;
		m_matrix[3][0] = _m30; m_matrix[3][1] = _m31; m_matrix[3][2] = _m32; m_matrix[3][3] = _m33;
	}

	myMatrix GetTransposed()
	{
		return myMatrix(m_matrix[0][0], m_matrix[0][1], m_matrix[0][2], m_matrix[0][3],
			m_matrix[1][0], m_matrix[1][1], m_matrix[1][2], m_matrix[1][3],
			m_matrix[2][0], m_matrix[2][1], m_matrix[2][2], m_matrix[2][3],
			m_matrix[3][0], m_matrix[3][1], m_matrix[3][2], m_matrix[3][3]);
	}
	/*myMatrix(const myMatrix& matrix)
	{
		for (int y = 0; y < 4; y++)
		{
			for (int x = 0; x < 4; x++)
			{
				m_matrix[x][y] = matrix[x][y];
			}
		}
	}*/

	~myMatrix() = default;

	T m_matrix[4][4];
};

using MyMatrix4 = myMatrix<float>;

class ViewMatrix : public MyMatrix4
{
public:
	ViewMatrix(const myVector3& camPos, const myVector3& targetPos, const myVector3& upVector)
	{
		/*myVector3 ZAxis = targetPos - camPos;
		ZAxis.getVectorNormalize();

		myVector3 XAxis = upVector ^ ZAxis;
		XAxis.getVectorNormalize();

		myVector3 YAxis*/
		myVector3 ZAxis = targetPos - camPos;
		ZAxis.getVectorNormalize();
		myVector3 XAxis = upVector ^ ZAxis;
		XAxis.getVectorNormalize();
		myVector3 YAxis = ZAxis ^ XAxis;


		for (uint32 RowIndex = 0; RowIndex < 3; ++RowIndex)
		{
			m_matrix[RowIndex][0] = (&XAxis.x)[RowIndex];
			m_matrix[RowIndex][1] = (&YAxis.x)[RowIndex];
			m_matrix[RowIndex][2] = (&ZAxis.x)[RowIndex];
			m_matrix[RowIndex][3] = 0.0f;
		}

		myVector3 negCameraPos = camPos;
		negCameraPos.x = negCameraPos.x * -1;
		negCameraPos.y = negCameraPos.y * -1;
		negCameraPos.z = negCameraPos.z * -1;

		m_matrix[3][0] = negCameraPos | XAxis;
		m_matrix[3][1] = negCameraPos | YAxis;
		m_matrix[3][2] = negCameraPos | ZAxis;
		m_matrix[3][3] = 1.0f;
	}
};

class ProjMatrix : public MyMatrix4
{
public:
	ProjMatrix(float HalfFOV, float Width, float Height, float minZ, float maxZ)
	{
		m_matrix[0][0] = 1.0f / tan(HalfFOV);
		m_matrix[0][1] = 0.0f;
		m_matrix[0][2] = 0.0f;
		m_matrix[0][3] = 0.0f;

		m_matrix[1][0] = 0.0f;
		m_matrix[1][1] = Width / tan(HalfFOV) / Height;
		m_matrix[1][2] = 0.0f;
		m_matrix[1][3] = 0.0f;

		m_matrix[2][0] = 0.0f;
		m_matrix[2][1] = 0.0f;
		m_matrix[2][2] = maxZ / (maxZ - minZ);
		m_matrix[2][3] = 0.0f;

		m_matrix[3][0] = 0.0f;
		m_matrix[3][1] = 0.0f;
		m_matrix[3][2] = -minZ * maxZ / (maxZ - minZ);
		m_matrix[3][3] = 0.0f;
	}

	/*ProjMatrix(float fov, float aspectRatio, float nearPlane, float farPlane)
	{
		float tanHalfFov = tan(fov / 2.0f);
		float zRange = nearPlane - farPlane;

		m_matrix[0][0] = 1.0f / (tanHalfFov); m_matrix[0][1] = 0.0f; m_matrix[0][2] = 0.0f; m_matrix[0][3] = 0.0f;
		m_matrix[1][0] = 0.0f; m_matrix[1][1] = 1.0f / tanHalfFov; m_matrix[1][2] = 0.0f; m_matrix[1][3] = 0.0f;
		m_matrix[2][0] = 0.0f; m_matrix[2][1] = 0.0f; m_matrix[2][2] =(-nearPlane - farPlane) / zRange; m_matrix[2][3] = 2.0f * farPlane * nearPlane / zRange;
		m_matrix[3][0] = 0.0f; m_matrix[3][1] = 0.0f; m_matrix[3][2] = 1.0f; m_matrix[3][3] = 0.0f;
	}*/
};