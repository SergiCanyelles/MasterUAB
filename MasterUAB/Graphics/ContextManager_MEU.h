#include <d3d11.h>

#pragma once
class CContextManager
{
private:
	ID3D11Device *l_D3DDevice; // esta clase, el device, nos sirve para crear objetos de DirectX
	ID3D11DeviceContext *l_DeviceContext; // el contexto nos va a servir para usar objetos de DirectX
	IDXGISwapChain *l_SwapChain; // la cadena de swap
	ID3D11RenderTargetView *l_RenderTargetView;

	double R;
	double G;
	double B;

public:
	CContextManager(void);
	~CContextManager(void);

	ID3D11Device *GetDevice() const{return l_D3DDevice;}
	ID3D11DeviceContext *GetContext() const{return l_DeviceContext;}
	IDXGISwapChain *GetSwap() const{return l_SwapChain;}

	LRESULT CreateContext(HWND &hWnd, int width, int height);

	int GetRenderTarget();
	
	void Draw();
	void Render();
	bool InitDevice();
};

