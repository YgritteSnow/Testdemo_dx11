#include <d3d11.h>
#include <d3dx11.h>
#include <Windows.h>
#include <tchar.h>
#include <xnamath.h>
#include <d3dcompiler.h>

#include "device.h"

//void main(){}
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd){
	if (FAILED(JJ_TEST_DEMO::InitWindow(hInstance, nShowCmd)))
		return 0;
	if (FAILED(JJ_TEST_DEMO::InitAll()))
		return 0;

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (WM_QUIT != msg.message){
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else{
			JJ_TEST_DEMO::Render();
		}
	}
	return 0;
}