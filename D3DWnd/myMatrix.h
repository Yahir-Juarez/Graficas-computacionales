#pragma once

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

