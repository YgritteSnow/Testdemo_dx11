#pragma once

namespace JJ_TEST_DEMO
{
	/************************************************************************/
	/* variables                                                            */
	/************************************************************************/
	extern const int g_width;
	extern const int g_height;

	extern ID3D11DeviceContext* g_immediateContext;
	extern ID3D11Device* g_device;

	extern IDXGISwapChain* g_swapChain;
	extern D3D_FEATURE_LEVEL g_featureLevel;
	extern ID3D11RenderTargetView* g_renderTargetView;

	extern HWND g_hwnd;
	extern HINSTANCE g_instance;

	/************************************************************************/
	/* functions                                                            */
	/************************************************************************/
	LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	HRESULT InitWindow(HINSTANCE hwnd, int nCmdShow);
	HRESULT InitAll();

	HRESULT InitDevice();

	void ClearDevice();
	void Render();
}