#include "CGrhaphicsManager.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <cassert>

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

	m_deviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);

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

	return nullptr;
}

SPtr<InputLayout> CGraphicsManager::createInputLayout(const Vector<D3D11_INPUT_ELEMENT_DESC>& layout,
	WPtr<VertexShader> pVS)
{
	auto pLayout = std::make_shared<InputLayout>();
	auto vs = pVS.lock();
	throwIfFailed(m_device->CreateInputLayout(&layout[0], 
		layout.size(), //rellenar hora 10:06
		vs->m_pBlob->GetBufferPointer(), 
		vs->m_pBlob->GetBufferSize(), 
		&pLayout->m_pLayout));

	return SPtr<InputLayout>();
}