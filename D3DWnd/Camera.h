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

	void RotatdeX(float grados);
	void RotatdeY(float grados);
	virtual ~Camera() = default;

	MyMatrix4 m_viewMatrix;
	MyMatrix4 m_projMatrix;
};

class CameraFPS : public Camera
{
public:
	CameraFPS() : m_camPos(0.0f, 0.0f, 0.0f),
		m_targetPos(0.0f, 0.0f, 1.0f),
		m_upVector(0.0f, 1.0f, 0.0f),
		m_HalfFOV(3.14159 / 4.0f),
		m_Width(1.0f),
		m_Height(1.0f),
		m_MinZ(0.1f),
		m_MaxZ(100.0f)
	{
		setViewData(m_camPos, m_targetPos, m_upVector);
		setProjData(m_HalfFOV, m_Width, m_Height, m_MinZ, m_MaxZ);
	}

	CameraFPS(const myVector3& camPos,
		const myVector3& targetPos,
		const myVector3& upVector,
		float HalfFOV,
		float Width,
		float Height,
		float minZ,
		float maxZ) : m_camPos(camPos), m_targetPos(targetPos), m_upVector(upVector),
		m_HalfFOV(HalfFOV), m_Width(Width), m_Height(Height), m_MinZ(minZ), m_MaxZ(maxZ)
	{	
		setViewData(m_camPos, m_targetPos, m_upVector);
		setProjData(m_HalfFOV, m_Width, m_Height, m_MinZ, m_MaxZ);
	}

	void moveForward(float amount)
	{
		myVector3 forward = m_targetPos - m_camPos;
		forward.Normalize();
		m_camPos = m_camPos + forward * amount;
		m_targetPos = m_targetPos + forward * amount;
		setViewData(m_camPos, m_targetPos, m_upVector);
	}

	void moveRight(float amount)
	{
		myVector3 forward = m_targetPos - m_camPos;
		myVector3 right = forward ^ m_upVector;
		right.Normalize();
		m_camPos = m_camPos + right * amount;
		m_targetPos = m_targetPos + right * amount;
		setViewData(m_camPos, m_targetPos, m_upVector);
	}

	void moveUp(float amount)
	{
		myVector3 forward = m_targetPos - m_camPos;
		myVector3 right = forward ^ m_upVector;
		myVector3 Up = right ^ forward;
		Up.Normalize();
		m_camPos = m_camPos + Up * amount;
		m_targetPos = m_targetPos + Up * amount;
		setViewData(m_camPos, m_targetPos, m_upVector);
	}

	void rotate(float yaw, float pitch)
	{
		myVector3 forward = m_targetPos - m_camPos;
		myVector3 right = forward ^ m_upVector;
		right.Normalize();
		myVector3 Up = right ^ forward;
		Up.Normalize();
	}

	myVector3 m_targetPos;
	myVector3 m_camPos;
	myVector3 m_upVector;
	float m_HalfFOV;
	float m_Width;
	float m_Height;
	float m_MinZ;
	float m_MaxZ;
};