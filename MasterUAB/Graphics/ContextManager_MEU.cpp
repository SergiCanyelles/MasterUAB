#include <d3d11.h>
#include <random>
#include "ContextManager.h"

CContextManager::CContextManager(void)
{
}


CContextManager::~CContextManager(void)
{
	l_D3DDevice->Release();
	l_DeviceContext->Release();
	l_SwapChain->Release();
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

	HRESULT ret = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &desc, &l_SwapChain, &l_D3DDevice, NULL, &l_DeviceContext);

	R = 0;
	G = 0;
	B = 0;

	if (FAILED(ret))
	{
		return S_FALSE;
	}else{
		return 1;
	}
}

/***************************************************************************************
**         Primero vamos a coger el rendertarget que ha definido la SwapChain         **
***************************************************************************************/
int CContextManager::GetRenderTarget()
{
	ID3D11Texture2D *pBackBuffer;
	if (FAILED(l_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)))
		return FALSE;
	HRESULT hr = l_D3DDevice->CreateRenderTargetView(pBackBuffer, NULL, &l_RenderTargetView);
	pBackBuffer->Release();
}


void CContextManager::Draw()
{
	D3D11_VIEWPORT viewPort = {0,0,800,600,0,1};
	R+= 0.005 * (rand() % 100);
	G+= 0.005 * (rand() % 100);
	B+= 0.005 * (rand() % 100);
	if (R > 1.0) {R=0;}
	if (G > 1.0) {G=0;}
	if (B > 1.0) {B=0;}
	float color[4] = {R,G,B,0};

	l_DeviceContext->ClearRenderTargetView(l_RenderTargetView, color);
	l_DeviceContext->OMSetRenderTargets(1, &l_RenderTargetView, nullptr);
	l_DeviceContext->RSSetViewports(1, &viewPort);
	l_SwapChain->Present(0,0);
}
