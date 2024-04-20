#pragma once

#include <d3d11.h>

#include "Prerequisitos.h"

#include <string>

//struct ID3DBlob;
//struct ID3D11VertexShader;
//struct ID3D11PixelShader;


class Shader
{
public:
	Shader() = default;
	virtual ~Shader()
	{
		SAFE_RELEASE(m_pBlob);
	}

	bool CompileFromFile(const String& fileName, const String& entryPoint, const String& shaderModel);

public:
	friend class CGraphicsManager;

	ID3DBlob* m_pBlob;
};

class VertexShader : public Shader
{
public:
	VertexShader() = default;
	~VertexShader() 
	{
		SAFE_RELEASE(m_pShader);
	}

public:
	friend class CGraphicsManager;

	ID3D11VertexShader* m_pShader = nullptr;
};

class PixelShader : public Shader
{
public:
	PixelShader() = default;
	~PixelShader()
	{
		SAFE_RELEASE(m_pShader);
	}

public:
	friend class CGraphicsManager;

	ID3D11PixelShader* m_pShader = nullptr;
};

