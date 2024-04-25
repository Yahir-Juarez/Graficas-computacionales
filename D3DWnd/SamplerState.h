#pragma once

#include <d3d11.h>

#include "Prerequisitos.h"

class SamplerState
{
public:
	SamplerState() = default;
	virtual ~SamplerState()
	{
		SAFE_RELEASE(m_pSampleState);
	}

protected:
	friend class CGraphicsManager;

	ID3D11SamplerState* m_pSampleState = nullptr;
};

