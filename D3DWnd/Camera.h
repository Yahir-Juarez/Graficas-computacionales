#pragma once

#include "myMatrix.h"

class Camera
{
public:
	Camera() = default;
	Camera(const myVector3& camPos,
		const myVector3& targetPos,
		const myVector3& upVector,
		float fov,
		float aspectRatio,
		float nearPlane,
		float farPlane) :
		m_viewMatrix(ViewMatrix(camPos, targetPos, upVector)),
		m_projMatrix(ProjMatrix(fov, aspectRatio, nearPlane, farPlane))
	{}

	void setViewData(const myVector3& camPos, const myVector3& targetPos, const myVector3& upVector)
	{
		m_viewMatrix = ViewMatrix(camPos, targetPos, upVector);
	}

	void setProjData(float fov, float aspectRatio, float nearPlane, float farPlane)
	{
		m_projMatrix = ProjMatrix(fov, aspectRatio, nearPlane, farPlane);
	}

	virtual ~Camera() = default;

	ViewMatrix m_viewMatrix;
	ProjMatrix m_projMatrix;
};