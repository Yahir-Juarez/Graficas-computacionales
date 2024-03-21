#include "CGrhaphicsManager.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <d3d11.h>
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

	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* deviceContext = nullptr;

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
									&device,
									&selectedFeatureLevel,
									&deviceContext));
	SAFE_RELEASE(pFactory);

	SAFE_RELEASE(deviceContext);
	SAFE_RELEASE(device);
}

CGraphicsManager::~CGraphicsManager()
{

}