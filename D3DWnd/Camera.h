#pragma once

#include "myMatrix.h"

class Camera
{
public:
	Camera() = default;
	Camera(const myVector3& camPos,
		const myVector3& targetPos,
		const myVector3& upVector,
		float HalfFOV,
		float Width,
		float Height,
		float minZ, 
		float maxZ)
		:
		m_viewMatrix(ViewMatrix(camPos, targetPos, upVector)),
		m_projMatrix(ProjMatrix( HalfFOV, Width, Height, minZ, maxZ))
	{}

	void setViewData(const myVector3& camPos, const myVector3& targetPos, const myVector3& upVector)
	{
		m_viewMatrix = ViewMatrix(camPos, targetPos, upVector);
	}

	void setProjData(float HalfFOV, float Width, float Height, float minZ, float maxZ)
	{
		m_projMatrix = ProjMatrix(HalfFOV, Width, Height, minZ, maxZ);
	}

	virtual ~Camera() = default;

	MyMatrix4 m_viewMatrix;
	MyMatrix4 m_projMatrix;
};