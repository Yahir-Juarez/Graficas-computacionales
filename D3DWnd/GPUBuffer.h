#pragma once

#include <d3d11.h>

#include "Prerequisitos.h"

class GPUBuffer
{
public:
	GPUBuffer() = default;
	virtual ~GPUBuffer()
	{
		GPUBuffer(m_pBuffer);
	}

protected:
	friend class CGraphicsManager;

	ID3D11Buffer* m_pBuffer = nullptr;
};

class VertexBuffer final : public GPUBuffer
{
public:
	VertexBuffer() = default;
	~VertexBuffer() = default;

protected:
	friend class CGraphicsManager;
	uint32 m_stride = 0;
};

class IndexBuffer final : public GPUBuffer
{
public:
	IndexBuffer() = default;
	~IndexBuffer() = default;

protected:
	friend class CGraphicsManager;
	uint32 m_dataFormat = DXGI_FORMAT_R32_UINT;
};

//constants buffer
