#pragma once
#include <d3d11.h>

#include "Prerequisitos.h"

class Texture
{
public:
	Texture() = default;
	virtual ~Texture()
	{
		SAFE_RELEASE(m_pShaderResourceView);
		SAFE_RELEASE(m_pDepthStencilView);
	}

protected:
	friend class CGraphicsManager;

	ID3D11ShaderResourceView* m_pShaderResourceView = nullptr;
	ID3D11DepthStencilView* m_pDepthStencilView = nullptr;
};

class Texture2D final : public Texture
{
public:
	Texture2D() = default;
	virtual ~Texture2D()
	{
		SAFE_RELEASE(m_pTexture);
	}

protected:
	friend class CGraphicsManager;
	ID3D11Texture2D* m_pTexture = nullptr;
};

