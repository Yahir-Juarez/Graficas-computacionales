#pragma once

#include <d3d11.h>

#include "Prerequisitos.h"

class InputLayout
{
public:
	InputLayout() = default;
	virtual ~InputLayout()
	{
		SAFE_RELEASE(m_pLayout);
	}

protected:
	friend class CGraphicsManager;

	ID3D11InputLayout* m_pLayout = nullptr;
};

