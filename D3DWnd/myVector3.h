#pragma once

class myVector3
{
public:
	myVector3() = default;
	~myVector3() = default;

	myVector3(float& x, float& y, float& z) { m_x = x; m_y = y; m_z = z; }
private:
	float m_x = 0;
	float m_y = 0;
	float m_z = 0;
};