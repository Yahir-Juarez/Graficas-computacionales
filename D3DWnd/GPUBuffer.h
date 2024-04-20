#pragma once

#include <d3d11.h>

#include "Prerequisitos.h"

class GPUBuffer
{
public:
	GPUBuffer() = default;
	virtual ~GPUBuffer()
	{
		SAFE_RELEASE(m_pBuffer);
	}

public:
	friend class CGraphicsManager;

	ID3D11Buffer* m_pBuffer = nullptr;
};

class VertexBuffer final : public GPUBuffer
{
public:
	VertexBuffer() = default;
	~VertexBuffer() = default;

public:
	friend class CGraphicsManager;
	uint32 m_stride = 0;
};

class IndexBuffer final : public GPUBuffer
{
public:
	IndexBuffer() = default;
	~IndexBuffer() = default;

public:
	friend class CGraphicsManager;
	uint32 m_dataFormat = DXGI_FORMAT_R32_UINT;
};

//constants buffer
