#include <cmath>

#include "Camera.h"
void Camera::RotatdeX(float grado)
{
	m_viewMatrix.m_matrix[1][1] = m_viewMatrix.m_matrix[1][1] * cos(grado);
	m_viewMatrix.m_matrix[1][2] = -m_viewMatrix.m_matrix[1][2] * sin(grado);
	m_viewMatrix.m_matrix[2][1] = m_viewMatrix.m_matrix[2][1] * sin(grado);
	m_viewMatrix.m_matrix[2][2] = m_viewMatrix.m_matrix[2][2] * cos(grado);
}

void Camera::RotatdeY(float grado)
{
	m_viewMatrix.m_matrix[0][1] = m_viewMatrix.m_matrix[0][1] * cos(grado);
	m_viewMatrix.m_matrix[0][2] = -m_viewMatrix.m_matrix[0][2] * sin(grado);
	m_viewMatrix.m_matrix[2][1] = m_viewMatrix.m_matrix[2][1] * sin(grado);
	m_viewMatrix.m_matrix[2][2] = m_viewMatrix.m_matrix[2][2] * cos(grado);
}
