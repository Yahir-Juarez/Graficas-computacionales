#pragma once

#include <d3d11.h>

#include "Prerequisitos.h"

#include "Shader.h"

#include "InputLayout.h"

#include "GPUBuffer.h"

#include "Texture2D.h"

#include "SamplerState.h"

class LinearColor
{
public:
	LinearColor(float R, float G, float B, float A)
		: r(R), g(G), b(B), a(A) {
	}
	~LinearColor()
	{
	};
private:
	float r;
	float g;
	float b;
	float a;
};


class CGraphicsManager
{
public:
	CGraphicsManager(void * scrHandle,
					 bool bFullScreen,
					 bool bAntialasing,
					 uint32 samplesPerPixel,
					 uint32 sampleQuality);
	~CGraphicsManager();

	void clearRenderTarget(ID3D11RenderTargetView* pTarget, LinearColor color);
	void clearRenderTargetView(ID3D11DepthStencilView* depthStencilView);
	void present();

	ID3D11RenderTargetView* getRenderTargetView() { return m_pRenderTargetView; }

	SPtr<InputLayout> createInputLayout(const Vector<D3D11_INPUT_ELEMENT_DESC>& layout, 
										WPtr<VertexShader> pVS);

	SPtr<VertexShader> createVertexShader(const Path& filaName,
										  const String& entryPoint, 
										  const String& shaderModel);

	SPtr<PixelShader> createPixelShader(const Path& filaName,
									    const String& entryPoint,
									    const String& shaderModel);

	template<typename T>
	inline SPtr<VertexBuffer> createVertexBuffer(const Vector<T>& vertices, uint32 usage = D3D11_USAGE_DEFAULT)
	{
		auto pVB = std::make_shared<VertexBuffer>();

		D3D11_BUFFER_DESC desc;
		memset(&desc, 0, sizeof(desc));
		desc.Usage = static_cast<D3D11_USAGE>(usage);
		desc.ByteWidth = static_cast<UINT>(vertices.size() * sizeof(T));
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = usage == D3D10_USAGE_DYNAMIC ? D3D11_CPU_ACCESS_WRITE : 0;
		desc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = &vertices[0];
		initData.SysMemPitch = 0;
		initData.SysMemSlicePitch = 0;

		pVB->m_stride = sizeof(T);

		HRESULT hr = 0;

		hr = m_device->CreateBuffer(&desc, &initData, &pVB->m_pBuffer);

		return pVB;
	}

	template<typename T>
	inline SPtr<IndexBuffer> createIndexBuffer(const Vector<T>& index, uint32 usage = D3D11_USAGE_DEFAULT)
	{
		if (index.empty()) { return nullptr; }

		auto pIB = std::make_shared<IndexBuffer>();

		D3D11_BUFFER_DESC desc;
		memset(&desc, 0, sizeof(desc));
		desc.Usage = static_cast<D3D11_USAGE>(usage);
		desc.ByteWidth = static_cast<UINT>(index.size() * sizeof(T));
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.CPUAccessFlags = usage == D3D10_USAGE_DYNAMIC ? D3D11_CPU_ACCESS_WRITE : 0;
		desc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = &index[0];
		initData.SysMemPitch = 0;
		initData.SysMemSlicePitch = 0;

		//pIB->m_dataFormat = sizeof(T);

		HRESULT hr = 0;

		hr = m_device->CreateBuffer(&desc, &initData, &pIB->m_pBuffer);

		if (sizeof(T) == 2)
		{
			pIB->m_dataFormat = DXGI_FORMAT_R16_UINT;
		}

		return pIB;
	}

	SPtr<ConstantBuffer> createConstantBuffer(const Vector<char>& buffer, uint32 usage = D3D11_USAGE_DEFAULT)
	{
		auto pCB = std::make_shared<ConstantBuffer>();

		D3D11_BUFFER_DESC desc;
		memset(&desc, 0, sizeof(desc));
		desc.Usage = static_cast<D3D11_USAGE>(usage);
		desc.ByteWidth = static_cast<UINT>(buffer.size());
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = usage == D3D10_USAGE_DYNAMIC ? D3D11_CPU_ACCESS_WRITE : 0;
		desc.MiscFlags = 0;

		//El D3D11_SUBRESOURCE_DATA lo utilizamos para copiar la nformacion cuando se crea

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = &buffer[0];
		initData.SysMemPitch = 0;
		initData.SysMemSlicePitch = 0;

		HRESULT hr = 0;

		hr = m_device->CreateBuffer(&desc, &initData, &pCB->m_pBuffer);

		return pCB;
	}

	SPtr<ConstantBuffer> createConsantBuffer(size_t bufferSize, void* pData = nullptr, uint32 usage = D3D11_USAGE_DEFAULT)
	{

		auto pCB = std::make_shared<ConstantBuffer>();

		D3D11_BUFFER_DESC desc;
		memset(&desc, 0, sizeof(desc));
		desc.Usage = static_cast<D3D11_USAGE>(usage);
		desc.ByteWidth = bufferSize;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = usage == D3D10_USAGE_DYNAMIC ? D3D11_CPU_ACCESS_WRITE : 0;
		desc.MiscFlags = 0;

		//El D3D11_SUBRESOURCE_DATA lo utilizamos para copiar la nformacion cuando se crea

		D3D11_SUBRESOURCE_DATA initData;
		if (pData)
		{
			initData.pSysMem = pData;
			initData.SysMemPitch = 0;
			initData.SysMemSlicePitch = 0;
		}
		HRESULT hr = 0;

		hr = m_device->CreateBuffer(&desc, pData ? &initData : nullptr, &pCB->m_pBuffer);

		return pCB;
	}

	void updateConstantBuffer(WPtr<ConstantBuffer> pCB, void* pData, size_t dataSize)
	{
		if (pCB.expired())
		{
			return;
		}

		auto pSCB = pCB.lock();

		if (!pSCB->m_pBuffer)
		{
			return;
		}

		m_deviceContext->UpdateSubresource(pSCB->m_pBuffer, 0, nullptr, pData, dataSize, 0);
	}
	//void updateConstatntBuffer(WPtr<Cons>)
	SPtr<SamplerState> CreateSampleState(D3D11_FILTER filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_MODE textureAddress = D3D11_TEXTURE_ADDRESS_WRAP);
	SPtr<Texture2D> createTexture2DFromFile(const Path& fileName, uint32 format = DXGI_FORMAT_B8G8R8A8_UNORM, uint32 usage = D3D11_USAGE_DEFAULT);
	SPtr<Texture2D> createTexture(uint32 Width, uint32 height, uint32 format = DXGI_FORMAT_B8G8R8A8_UNORM, uint32 usage = D3D11_USAGE_DEFAULT, uint32 bindFlag = D3D11_BIND_SHADER_RESOURCE);
	

	ID3D11DeviceContext* getDC() { return m_deviceContext; } //minuto 10:20
	ID3D11DepthStencilView* getMainDSV(){ return m_pDethStencil->m_pDepthStencilView; }

	void setRenderTargets(UINT numViews = 1, ID3D11DepthStencilView* depthStencilView = nullptr);
	void setInputLayout(SPtr<InputLayout> inputLayout); //
	void setVertexBuffers(SPtr <VertexBuffer> vertexBuffer,UINT& offset);//--Cadenas//
	void setIndexBuffers(SPtr <IndexBuffer> indexBuffer);//--Cadenas//
	void setPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);//
	void setVertexShader(SPtr<VertexShader> pShader);
	void setPixelShader(SPtr<PixelShader> pShader);
	void setShaderResources(WPtr<Texture> shaderResourceView, UINT32 startSlot = 0, UINT32 numViews = 1);
	void setSamplers(WPtr<SamplerState> sampleState, UINT32 startSlot = 0, UINT32 numSamplers = 1);
	void Draw(UINT count, UINT startVertexLocation = 0);
	void DrawIndex(UINT count, UINT startIndexLocation = 0, UINT BaseLocation = 0);

	void vsSetConstantBuffers(SPtr<ConstantBuffer> bufferID3D11, UINT32 StartSlot = 0, UINT32 NumBuffers = 1);
private:
	bool m_bFullScreen;
	DXGI_SAMPLE_DESC m_multiConfig;
	IDXGISwapChain* m_pSwapChain = nullptr;
	ID3D11Device* m_device = nullptr;

	ID3D11DeviceContext* m_deviceContext = nullptr;
	ID3D11Texture2D* m_pBackBuffer = nullptr;
	SPtr<Texture2D> m_pDethStencil;
	
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;
};