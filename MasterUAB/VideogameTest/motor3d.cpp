
/******************************************************************************************************************************************
/******************************************************************************************************************************************
**                                                                                                                                       **
**                                                             Inicialización                                                            **
**                                                                                                                                       **
/******************************************************************************************************************************************
/******************************************************************************************************************************************


/**********************************************************************
**  Lo primero que haremos será inicializar una ventana en Windows:  **
**********************************************************************/

{
// Definir la "classe" que sera nuestra ventana
WNDCLASSEX wc = {
 sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, APPLICATION_NAME, NULL
 };
	RegisterClassEx(&wc);

	// Calcular el tamano de nuestra ventana
	RECT rc = {
			0, 0, WIDTH_APPLICATION, HEIGHT_APPLICATION
		};
	AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE);

	// Crear la ventana en si
	HWND hWnd = CreateWindow( APPLICATION_NAME, APPLICATION_NAME, WS_OVERLAPPEDWINDOW, 100, 100, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, wc.hInstance, NULL);

	// NOTA: hWND es el handler a nuestra ventana. Es importante que nos lo guardemos.

	// TODO Crear el contexto DIRECTX
	CreateContext( hWnd, WIDTH_APPLICATION, HEIGHT_APPLICATION);

	// Mostrar la ventana
	ShowWindow(hWnd, SW_SHOWDEFAULT);

	// TODO Crear el back buffer
	CreateBackBuffer( hWnd, WIDTH_APPLICATION, HEIGHT_APPLICATION );

	UpdateWindow(hWnd);
}

/*********************************************************************
**  Para que todo esto funcione, necesitaremos definir              **
**  la función que recibe los mensages de Windows                   **
*********************************************************************/


LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_SIZE:
		if (m_D3DDevice != nullptr && wParam != SIZE_MINIMIZED)
		{
			// TODO destruir el back buffer
			// TODO actualizar el tamano de la "Swap Chain"
			// TODO crear de nuevo los back buffers
		}
		return 0;
	case WM_DESTROY:
		// Decirle a windows que nos envie el mensage de cerrar a nuestro bucle principal
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			return 0;
			break;
		}
		break;
	}
	}
	// Todo lo que no conozcamos, se lo devolvemos a Windows
	return DefWindowProc(hWnd, msg, wParam, lParam);
}


/**********************************************************************************
**  Finalmente vamos a necesitar crear el bucle principal de nuestra aplicación  **
**********************************************************************************/

{
	MSG msg;
	ZeroMemory(&msg, sizeof(msg)); // empezamos a zero

	// Alternativa:
	//MSG = {};

	while (msg.message != WM_QUIT)
	{
		// miramos si windows tiene un mensage para nosotros
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) 
		{
			// se lo devolvemos a windows
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			
			// NOTA: si el mensage era "WM_QUIT", se ha quedado registrado en "msg" y vamos a salir del bucle
		}
		else
		{
			// TODO
			Update();
			Render();
		}
	}
	// TODO finalizacion de todos los objetos

	return (int) msg.wParam;
}

/******************************************************************************************************************************************
/******************************************************************************************************************************************
**                                                                                                                                       **
**                                              Crear Estructuras básicas de DirectX                                                     **
**                                                                                                                                       **
/******************************************************************************************************************************************
/******************************************************************************************************************************************

/***************************************************************************************
**  Antes de nada, vamos a tener que incluir las cabezeras y las librerias de DirectX **
***************************************************************************************/


#include <d3d11.h>
#include <d3dx11.h>

#ifdef NDEBUG
#pragma comment(lib,"d3dx11.lib")
#else
#pragma comment(lib,"d3dx11d.lib")
#endif

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxerr.lib")
#pragma comment(lib,"dxguid.lib")


/***************************************************************************************
**  Para pintar en DirectX, primero necesitamos crear un contexto de DirectX.         **
**  Para ello vamos a definir la "cadena de intercambio", o sea, cómo se va a         **
**  comportar nuestro programa cada "frame"                                           **
***************************************************************************************/

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
	// TODO:
	//desc.BufferDesc.Width;
	//desc.BufferDesc.Height;
	//desc.BufferDesc.RefreshRate.Numerator;
	//desc.BufferDesc.RefreshRate.Denominator;
	//desc.OutputWindow;
	//desc.SampleDesc.Count;
	//desc.SampleDesc.Quality;
	//desc. ????

	// Que DirectX queremos
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);


	ID3D11Device *l_D3DDevice; // esta clase, el device, nos sirve para crear objetos de DirectX
	ID3D11DeviceContext *l_DeviceContext; // el contexto nos va a servir para usar objetos de DirectX
	IDXGISwapChain *l_SwapChain; // la cadena de swap

	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &desc, &l_SwapChain, &l_D3DDevice, NULL, &l_DeviceContext)))
	{
		return S_FALSE;
	}
}

/***************************************************************************************
**         Primero vamos a coger el rendertarget que ha definido la SwapChain         **
***************************************************************************************/

{
	ID3D11RenderTargetView*	l_RenderTargetView;

	ID3D11Texture2D *pBackBuffer;
	if (FAILED(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)))
		return FALSE;
	HRESULT hr = m_D3DDevice->CreateRenderTargetView(pBackBuffer, NULL, &l_RenderTargetView);
	pBackBuffer->Release();
		
}

/***************************************************************************************
**                  	Luego vamos a añadir un buffer de profundidad.                **
***************************************************************************************/

{
	ID3D11Texture2D* l_DepthStencil;
	ID3D11DepthStencilView*	l_DepthStencilView;

	{
		D3D11_TEXTURE2D_DESC desc = {};
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		// TODO:
		//desc.Width;
		//desc.Height;
		//desc.MipLevels;
		//desc.ArraySize;
		//desc.SampleDesc.Count;
		//desc.SampleDesc.Quality;
		hr = l_D3DDevice->CreateTexture2D(&desc, NULL, &l_DepthStencil);
		if (FAILED(hr))
			return hr;
	}

	{
		D3D11_DEPTH_STENCIL_VIEW_DESC desc = {};
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 0;
		// TODO
		//desc.Format;
		hr = l_D3DDevice->CreateDepthStencilView(l_DepthStencil, &desc, &l_DepthStencilView);
		if (FAILED(hr))
			return hr;
	}
}

/***************************************************************************************
**					A partir de aquí ya podemos renderizar                            **
***************************************************************************************/


// Limpiar el framebuffer:
// ID3D11DeviceContext::ClearRenderTargetView
// ID3D11DeviceContext::ClearDepthStencilView

// definir a que target vamos a pintar:
// ID3D11DeviceContext::OMSetRenderTargets

// definir el viewport:
// ID3D11DeviceContext::RSSetViewports

// TODO renderizar aqui

// presentar lo pintado:
// IDXGISwapChain::Present


/***************************************************************************************
**  Si queremos un triangulo, necesitamos preparar varias cosas:                      **
**    - La primera, los shaders (mirar fichero adjunto                                **
**	  - Luego, los buffers                                                            **
***************************************************************************************/

{
	float vertices[3 * 7] =
		{
			-0.5f, -0.5f, 0.5f,     1.0f, 0.0f, 0.0f, 1.0f,
			+0.0f, +0.5f, 0.5f,     0.0f, 1.0f, 0.0f, 1.0f,
			+0.5f, -0.5f, 0.5f,     0.0f, 0.0f, 1.0f, 1.0f,
		};


	ID3D11Buffer* l_VertexBuffer;

	D3D11_BUFFER_DESC desc = {}
	desc.Usage=D3D11_USAGE_DEFAULT;
	desc.BindFlags=D3D11_BIND_VERTEX_BUFFER;
	// TODO
	// desc.ByteWidth;
	// desc.CPUAccessFlags;
	D3D11_SUBRESOURCE_DATA InitData = {};
	InitData.pSysMem=vertices;
	// ID3D11DeviceContext::CreateBuffer
}

/***************************************************************************************
**  				Finalmente podremos pintar dicho buffer       		              **
***************************************************************************************/
{
	static CEffectParameters Parameters;
	
	Parameters.m_World.SetIdentity();
	Parameters.m_View.SetIdentity();
	Parameters.m_Projection.SetIdentity();
	Parameters.m_BaseColor = CColor(1,1,1,1);
	
	//   ID3D11DeviceContext::IASetVertexBuffers;
	//   ID3D11DeviceContext::IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//   ID3D11DeviceContext::IASetInputLayout;
	//   ID3D11DeviceContext::VSSetShader;
	//   ID3D11DeviceContext::UpdateSubresource(  ConstantBuffer  , 0, NULL, &Parameters, 0, 0 );
	//   ID3D11DeviceContext::VSSetConstantBuffers;
	//   ID3D11DeviceContext::PSSetShader;

	//   ID3D11DeviceContext::Draw(3, 0);
}

{
	// TODO draw indexed
	// ID3D11DeviceContext::IASetIndexBuffer
	// ID3D11DeviceContext::DrawIndexed(3, 0);
}

/***************************************************************************************
**				  				Definir estados de render		       		          **
***************************************************************************************/

// entender la pipeline:
// https://msdn.microsoft.com/en-us/library/windows/desktop/ff476882(v=vs.85).aspx

{
	// D3D11_RASTERIZER_DESC
	// ID3D11Device::CreateRasterizerState
	
	// D3D11_DEPTH_STENCIL_DESC
	// ID3D11Device::CreateDepthStencilState
	
	// D3D11_BLEND_DESC1
	// ID3D11Device::CreateBlendState1

	// ID3D11DeviceContext::RSSetState
	// ID3D11DeviceContext::OMSetDepthStencilState
	// ID3D11DeviceContext::OMSetBlendState
}
