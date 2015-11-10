#include <d3d11.h>
#include "ContextManager.h"

CContextManager::CContextManager(void)
{
}


CContextManager::~CContextManager(void)
{
}

LRESULT CContextManager::CreateContext(HWND &hWnd, int width, int height)
{
	// Tendremos que crear y rellenar una estructura de este tipo
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	// o
	//DXGI_SWAP_CHAIN_DESC desc = {};
	desc.BufferCount = 1;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.Windowed = TRUE;
	desc.BufferDesc.Width = width;
	desc.BufferDesc.Height = height;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.OutputWindow = hWnd;
	desc.SampleDesc.Count = 1;
	//desc.SampleDesc.Quality;
	//desc. ????

	// Que DirectX queremos
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_10_1,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	ID3D11Device *l_D3DDevice; // esta clase, el device, nos sirve para crear objetos de DirectX
	ID3D11DeviceContext *l_DeviceContext; // el contexto nos va a servir para usar objetos de DirectX
	IDXGISwapChain *l_SwapChain; // la cadena de swap

	HRESULT ret = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &desc, &l_SwapChain, &l_D3DDevice, NULL, &l_DeviceContext);

	if (FAILED(ret))
	{
		return S_FALSE;
	}else{
		return 1;
	}
}