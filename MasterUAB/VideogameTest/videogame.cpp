#include <Windows.h>
#include <d3d11.h>
#include <ContextManager.h>
#include "Application.h"
#include "DebugRender.h"

#pragma comment(lib,"d3d11.lib")

#define APPLICATION_NAME	"VIDEOGAMETEST"

	ID3D11Device *l_D3DDevice; // esta clase, el device, nos sirve para crear objetos de DirectX
	ID3D11DeviceContext *l_DeviceContext; // el contexto nos va a servir para usar objetos de DirectX
	IDXGISwapChain *l_SwapChain; // la cadena de swap
	int WIDTH_APPLICATION = 800;
	int HEIGHT_APPLICATION = 600;
	LRESULT CreateContext(HWND &hWnd, int width, int height);


//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{

  switch( msg )
  {
  case WM_DESTROY:
    {
      PostQuitMessage( 0 );
      return 0;
    }
    break;
  case WM_KEYDOWN:
    {
      switch( wParam )
      {
      case VK_ESCAPE:
        //Cleanup();
        PostQuitMessage( 0 );
        return 0;
        break;
      }
    }
    break;
  }//end switch( msg )

  return DefWindowProc( hWnd, msg, wParam, lParam );
}

//-----------------------------------------------------------------------
// WinMain
//-----------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpCmdLine, int _nCmdShow)
{
	int width = 800;
	int height = 600;

	// Register the window class
	WNDCLASSEX wc = {	sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, APPLICATION_NAME, NULL };

	RegisterClassEx( &wc );

  	l_D3DDevice = 0;
	l_DeviceContext = 0;
	l_SwapChain = 0;

	// Calcular el tamano de nuestra ventana
	RECT rc = {0, 0, WIDTH_APPLICATION, HEIGHT_APPLICATION};
	AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE);
	
	//width =  rc.right - rc.left;
	//height =  rc.bottom - rc.top;

	// Create the application's window
	HWND hWnd = CreateWindow(	APPLICATION_NAME, APPLICATION_NAME, WS_OVERLAPPEDWINDOW, 100, 100, 800, 600, NULL, NULL, wc.hInstance, NULL );
  	
	// Añadir aquí el Init de la applicacioón
	CContextManager context;
	context.CreateContext(hWnd, width, height);

	ShowWindow( hWnd, SW_SHOWDEFAULT );

	context.CreateBackBuffer(hWnd,800,600); 
	context.InitStates();
	CDebugRender debugRender(context.GetDevice());

	CApplication application(&debugRender, &context);
		
	UpdateWindow(hWnd);
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));



  // Añadir en el while la condición de salida del programa de la aplicación
	
	DWORD m_PreviousTime = timeGetTime();
  while( msg.message != WM_QUIT )
  {
    if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
    {
      TranslateMessage( &msg );
      DispatchMessage( &msg );
    }
    else
    {
		DWORD l_CurrentTime = timeGetTime();
		float m_ElapsedTime = (float)(l_CurrentTime - m_PreviousTime)*0.001f;
		m_PreviousTime = l_CurrentTime;

		application.Update(m_ElapsedTime);
		application.Render();
		//contextManager->Draw();
       // Main loop: Añadir aquí el Update y Render de la aplicación principal
    }
  }
  UnregisterClass( APPLICATION_NAME, wc.hInstance );

  // Añadir una llamada a la alicación para finalizar/liberar memoria de todos sus datos

  return 0;
}

/***************************************************************************************
**  Para pintar en DirectX, primero necesitamos crear un contexto de DirectX.         **
**  Para ello vamos a definir la "cadena de intercambio", o sea, cómo se va a         **
**  comportar nuestro programa cada "frame"                                           **
***************************************************************************************/
