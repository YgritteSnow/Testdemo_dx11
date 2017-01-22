#include <d3d11.h>
#include <d3dx11.h>

LRESULT CALLBACK winProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
HRESULT Init();
HRESULT Render();
HRESULT Destroy();

INT WINAPI winMain(HINSTANCE hInst, HINSTANCE, LPWSTR lpCmdLine, int nCmdShow)
{
	if (FAILED(Init()))
		return;
	
	return S_OK;
}