#pragma once

class myMatrix
{
public:
	myMatrix()
	{
		for (int y = 0; y < 4; y++)
		{
			for (int x = 0; x < 4; x++)
			{
				m_matrix[y][x] = 0.0f;
			}
		}
	}
	/*template <typename T>
	myMatrix(T matrix[4][4])
	{
		m_matrix = matrix
	}*/

	~myMatrix() = default;

	float m_matrix[4][4];
};