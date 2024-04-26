#include "CGrhaphicsManager.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <cassert>

#include "ImageProcess/Image.h"

inline void
throwIfFailed(HRESULT hr)
{
	if (FAILED(hr))
	{
		assert(false && "Something went horribly wrong!!!");
	}
}
CGraphicsManager::CGraphicsManager(void* scrHandle,
	bool bFullScreen,
	bool bAntialasing,
	uint32 samplesPerPixel,
	uint32 sampleQuality)
	: m_bFullScreen(bFullScreen)
{
	auto hWnd = reinterpret_cast<HWND>(scrHandle);
	RECT rc;
	GetClientRect(hWnd, &rc);

	Vector<IDXGIAdapter*> vecAdapters;

	IDXGIAdapter* pAdapter = nullptr;
	IDXGIAdapter1* pAdapter1 = nullptr;

	IDXGIFactory1* pFactory = nullptr;

	//Enumerate all the graphics adapters
	CreateDXGIFactory1(__uuidof(IDXGIFactory), (void**)&pFactory);

	uint32 iAdapter = 0;
	while (DXGI_ERROR_NOT_FOUND != pFactory->EnumAdapters(iAdapter, &pAdapter))
	{
		DXGI_ADAPTER_DESC1 aDesc1;

		pAdapter1 = nullptr;
		pAdapter->QueryInterface(__uuidof(IDXGIAdapter1), (void**)&pAdapter1);
		pAdapter1->GetDesc1(&aDesc1);

		vecAdapters.push_back(pAdapter);
		++iAdapter;
		SAFE_RELEASE(pAdapter1);
	}

	//Instialice D3D Device

	Vector<D3D_FEATURE_LEVEL> featureLevels = { D3D_FEATURE_LEVEL_11_0,
												D3D_FEATURE_LEVEL_10_1,
												D3D_FEATURE_LEVEL_10_0,
												D3D_FEATURE_LEVEL_9_3 };
	D3D_FEATURE_LEVEL selectedFeatureLevel;

	uint32 deviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if _DEBUG
	deviceFlags != D3D11_CREATE_DEVICE_DEBUG;
#endif

	//Create a device and inmediate device context
	throwIfFailed(D3D11CreateDevice(vecAdapters[0],
									D3D_DRIVER_TYPE_UNKNOWN,
									nullptr,
									deviceFlags,
									&featureLevels[0],
									static_cast<UINT>(featureLevels.size()),
									D3D11_SDK_VERSION,
									&m_device,
									&selectedFeatureLevel,
									&m_deviceContext));
	//create swape chain 

	DXGI_SWAP_CHAIN_DESC scDesc;
	memset(&scDesc, 0, sizeof(scDesc));

	scDesc.OutputWindow = hWnd;
	scDesc.Windowed = !m_bFullScreen;
	if (!m_bFullScreen)
	{
		scDesc.BufferDesc.Width = rc.right;
		scDesc.BufferDesc.Height = rc.bottom;
		scDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	}
	scDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;

	m_multiConfig.Count = 1;
	m_multiConfig.Quality = 0;

	if (bAntialasing)
	{
		m_multiConfig.Count = samplesPerPixel;
		m_multiConfig.Count = sampleQuality;
	}

	scDesc.SampleDesc = m_multiConfig;
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.BufferCount = 2;
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	scDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	IDXGIDevice* dxgiDevice = nullptr;
	throwIfFailed(m_device->QueryInterface(__uuidof(IDXGIDevice),
		reinterpret_cast<void**>(&dxgiDevice)));

	IDXGIAdapter* dxgiAdapter = nullptr;
	dxgiDevice->GetAdapter(&dxgiAdapter);

	IDXGIFactory* dxgiFactory = nullptr;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&dxgiFactory));

	throwIfFailed(dxgiFactory->CreateSwapChain(m_device, &scDesc, &m_pSwapChain));




	//Get Backbuffer Interface
	//Create render target window

	
	throwIfFailed(m_pSwapChain->GetBuffer(0,
										  __uuidof(ID3D11Texture2D), 
										  reinterpret_cast<LPVOID*>(&m_pBackBuffer)));

	
	throwIfFailed(m_device->CreateRenderTargetView(m_pBackBuffer,
												   nullptr,
												   &m_pRenderTargetView));

	m_pDethStencil = createTexture(scDesc.BufferDesc.Width, scDesc.BufferDesc.Height, DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_USAGE_DEFAULT, D3D11_BIND_DEPTH_STENCIL);


	//Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = static_cast<FLOAT>(scDesc.BufferDesc.Width);
	vp.Height = static_cast<FLOAT>(scDesc.BufferDesc.Height);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_deviceContext->RSSetViewports(1, &vp);
	//Remove

	m_deviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDethStencil->m_pDepthStencilView);

	//Release all objects
	SAFE_RELEASE(pFactory);
	SAFE_RELEASE(dxgiFactory);
	SAFE_RELEASE(dxgiAdapter);
	SAFE_RELEASE(dxgiAdapter);
}

CGraphicsManager::~CGraphicsManager()
{
	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pBackBuffer);
	SAFE_RELEASE(m_pSwapChain);

	SAFE_RELEASE(m_deviceContext);
	SAFE_RELEASE(m_device);
}

void CGraphicsManager::clearRenderTarget(ID3D11RenderTargetView* pTarget, LinearColor color)
{
	m_deviceContext->ClearRenderTargetView(pTarget, reinterpret_cast<FLOAT*>(&color));
}

void CGraphicsManager::clearRenderTargetView(ID3D11DepthStencilView* depthStencilView)
{
	m_deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}
void CGraphicsManager::present()
{
	m_pSwapChain->Present(0, 0);
}


#include <d3dcompiler.h>

bool compileShaderFromFile(const String& fileName, const String& entryPoint, const String& shaderModel, ID3DBlob** pBlob)
{
	HRESULT hr = S_OK;
	int32 shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if _DEBUG
	shaderFlags |= D3DCOMPILE_DEBUG;
#endif

	auto beg = (Path::const_iterator)fileName.begin();
	auto end = (Path::const_iterator)fileName.end();
	Wstring wFileName(beg, end);

	ID3DBlob* pErrorBlob = nullptr;
	hr = D3DCompileFromFile(wFileName.c_str(), nullptr, nullptr, entryPoint.c_str(), shaderModel.c_str(), shaderFlags, 0, pBlob, &pErrorBlob);

	if (FAILED(hr))
	{
		if (nullptr != pErrorBlob)
		{
			String errStr(reinterpret_cast<char*>(pErrorBlob->GetBufferPointer()));
			SAFE_RELEASE(pErrorBlob);
		}
		return false;
	}

	return true;
}

SPtr<VertexShader> CGraphicsManager::createVertexShader(const Path& fileName, const String& entryPoint, const String& shaderModel)
{
	auto myShader = std::make_shared<VertexShader>();

	if (!compileShaderFromFile(fileName, entryPoint, shaderModel, &myShader->m_pBlob))
	{
		return nullptr;
	}

	throwIfFailed(m_device->CreateVertexShader(myShader->m_pBlob->GetBufferPointer(), 
		myShader->m_pBlob->GetBufferSize(),
		nullptr, &myShader->m_pShader));

	return myShader;
}

SPtr<PixelShader> CGraphicsManager::createPixelShader(const Path& fileName, const String& entryPoint, const String& shaderModel)
{
	auto myShader = std::make_shared<PixelShader>();

	if (!compileShaderFromFile(fileName, entryPoint, shaderModel, &myShader->m_pBlob))
	{
		return nullptr;
	}

	throwIfFailed(m_device->CreatePixelShader(myShader->m_pBlob->GetBufferPointer(),
		myShader->m_pBlob->GetBufferSize(),
		nullptr, &myShader->m_pShader));

	return myShader;
}

SPtr<InputLayout> CGraphicsManager::createInputLayout(const Vector<D3D11_INPUT_ELEMENT_DESC>& layout,
	WPtr<VertexShader> pVS)
{
	auto pLayout = std::make_shared<InputLayout>();
	auto vs = pVS.lock();
	throwIfFailed(m_device->CreateInputLayout(&layout[0], 
		static_cast<UINT>(layout.size()), //rellenar hora 10:06
		vs->m_pBlob->GetBufferPointer(), 
		vs->m_pBlob->GetBufferSize(), 
		&pLayout->m_pLayout));

	return pLayout;
}

SPtr<SamplerState> CGraphicsManager::CreateSampleState(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE textureAddress)
{
	auto pSS = std::make_shared<SamplerState>();
	D3D11_SAMPLER_DESC sampDesc;
	memset(&sampDesc, 0, sizeof(sampDesc));
	sampDesc.Filter = filter;
	sampDesc.AddressU = textureAddress;
	sampDesc.AddressV = textureAddress;
	sampDesc.AddressW = textureAddress;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	throwIfFailed(m_device->CreateSamplerState(&sampDesc, &pSS->m_pSampleState));
	return pSS;
}


void CGraphicsManager::setRenderTargets(UINT numViews, ID3D11DepthStencilView* depthStencilView)
{
	m_deviceContext->OMSetRenderTargets(numViews, &m_pRenderTargetView, depthStencilView);
}

void CGraphicsManager::setInputLayout(SPtr<InputLayout> inputLayout)
{
	m_deviceContext->IASetInputLayout(inputLayout->m_pLayout);
}

void CGraphicsManager::setVertexBuffers(SPtr <VertexBuffer> vertexBuffer, Vector <UINT>& offset)
{
	m_deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer->m_pBuffer, &vertexBuffer->m_stride, &offset[0]);
}

void CGraphicsManager::setIndexBuffers(SPtr <IndexBuffer> indexBuffer)
{
	m_deviceContext->IASetIndexBuffer(indexBuffer->m_pBuffer, DXGI_FORMAT_R32_UINT, 0);
}

void CGraphicsManager::setPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY topology)
{
	m_deviceContext->IASetPrimitiveTopology(topology);
}

void CGraphicsManager::setVertexShader(SPtr<VertexShader> pShader)
{
	m_deviceContext->VSSetShader(pShader->m_pShader, nullptr, 0);
}

void CGraphicsManager::setPixelShader(SPtr<PixelShader> pShader)
{
	m_deviceContext->PSSetShader(pShader->m_pShader, nullptr, 0);
}

void CGraphicsManager::setShaderResources(WPtr<Texture> shaderResourceView, UINT32 startSlot, UINT32 numViews)
{
	auto pSR = shaderResourceView.lock();
	m_deviceContext->PSSetShaderResources(startSlot, numViews, &pSR->m_pShaderResourceView);
}

void CGraphicsManager::setSamplers(WPtr<SamplerState> sampleState, UINT32 startSlot, UINT32 numSamplers)
{
	auto pSS = sampleState.lock();
	m_deviceContext->PSSetSamplers(startSlot, numSamplers, &pSS->m_pSampleState);
}

void CGraphicsManager::Draw(UINT count, UINT startVertexLocation)
{
	m_deviceContext->Draw(count, startVertexLocation);
}

void CGraphicsManager::DrawIndex(UINT count, UINT startIndexLocation, UINT BaseLocation)
{
	m_deviceContext->DrawIndexed(count, startIndexLocation, BaseLocation);
}

SPtr<Texture2D> CGraphicsManager::createTexture2DFromFile(const Path& fileName, uint32 format, uint32 usage)
{
	g_CodecMan.AddCodec(new BMPCodec());
	Image newTexture;
	newTexture.CreateFromImageFile(fileName);

	auto pT2D = createTexture(newTexture.m_width, newTexture.m_height);

	/*D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = newTexture.m_pixelData.data();
	initData.SysMemPitch = newTexture.m_pitch;
	initData.SysMemSlicePitch = 0;*/

	m_deviceContext->UpdateSubresource(pT2D->m_pTexture, 0, nullptr, newTexture.m_pixelData.data(), newTexture.m_pitch, 0);
	/*m_device->CreateTexture2D(&descDepth, &initData, &pT2D->m_pTexture);*/
	return pT2D;
}

SPtr<Texture2D> CGraphicsManager::createTexture(uint32 Width, uint32 Height, uint32 format, uint32 usage, uint32 bindFlag)
{
	auto pT2D = std::make_shared<Texture2D>();

	D3D11_TEXTURE2D_DESC textureDesc;
	memset(&textureDesc, 0, sizeof(textureDesc));
	textureDesc.Width = Width;
	textureDesc.Height = Height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = static_cast<DXGI_FORMAT>(format);
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = static_cast<D3D11_USAGE>(usage);
	textureDesc.BindFlags = bindFlag;
	textureDesc.CPUAccessFlags = usage == D3D10_USAGE_DYNAMIC ? D3D11_CPU_ACCESS_WRITE : 0;
	textureDesc.MiscFlags = 0;

	throwIfFailed(m_device->CreateTexture2D(&textureDesc, nullptr, &pT2D->m_pTexture));


	if ((bindFlag & D3D11_BIND_SHADER_RESOURCE) == D3D11_BIND_SHADER_RESOURCE)
	{
		CD3D11_SHADER_RESOURCE_VIEW_DESC shaderRVD;

		memset(&shaderRVD, 0, sizeof(shaderRVD));
		shaderRVD.Format = textureDesc.Format;
		shaderRVD.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderRVD.Texture2D.MipLevels = textureDesc.MipLevels;
		shaderRVD.Texture2D.MostDetailedMip = 0;

		throwIfFailed(m_device->CreateShaderResourceView(pT2D->m_pTexture, &shaderRVD, &pT2D->m_pShaderResourceView));
	}
	if ((bindFlag & D3D11_BIND_DEPTH_STENCIL) == D3D11_BIND_DEPTH_STENCIL)\
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		memset(&descDSV, 0, sizeof(descDSV));
		descDSV.Format = textureDesc.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		throwIfFailed(m_device->CreateDepthStencilView(pT2D->m_pTexture, &descDSV, &pT2D->m_pDepthStencilView));
	}
	return pT2D;
}