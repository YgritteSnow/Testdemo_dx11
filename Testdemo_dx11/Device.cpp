#include <d3d11.h>
#include <d3dx11.h>
#include <Windows.h>
#include <tchar.h>
#include <d3dcompiler.h>

#include "Device.h"
#include "ModelManager.h"

namespace JJ_TEST_DEMO
{
	/************************************************************************/
	/* variables                                                            */
	/************************************************************************/
	const int g_width = 500;
	const int g_height = 300;

	ID3D11DeviceContext* g_immediateContext = NULL;
	ID3D11Device* g_device = NULL;
	
	IDXGISwapChain* g_swapChain = NULL;
	D3D_FEATURE_LEVEL g_featureLevel;
	ID3D11RenderTargetView* g_renderTargetView = NULL;
	ID3D11DepthStencilView* g_depthStencilView = NULL;

	HWND g_hwnd = NULL;
	HINSTANCE g_instance = NULL;

	/************************************************************************/
	/* functions                                                            */
	/************************************************************************/
	HRESULT InitWindow(HINSTANCE hinst, int nCmdShow) {
		WNDCLASSEX wc = {
			sizeof(WNDCLASSEX),
			CS_HREDRAW | CS_VREDRAW,
			wndProc,
			0, 0,
			hinst,
			NULL, NULL, NULL, NULL,
			_T("myclass"),
			NULL
		};
		if (!RegisterClassEx(&wc))
			return E_FAIL;

		RECT rc = { 0, 0, g_width, g_height };
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

		g_hwnd = CreateWindow(_T("myclass"), _T("caption~~"),
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			g_width, g_height,
			NULL, NULL, hinst, NULL
		);
		if (g_hwnd == NULL)
			return E_FAIL;

		ShowWindow(g_hwnd, nCmdShow);
		return S_OK;
	}

	HRESULT InitAll() {
		if (FAILED(InitDevice())) {
			ClearDevice();
			return E_FAIL;
		}
		if (FAILED(ModelManager::Init(g_device, g_immediateContext))) {
			ClearDevice();
			return E_FAIL;
		}
		return S_OK;
	}

	HRESULT InitDevice() {
		// init swap chain
		D3D_FEATURE_LEVEL features[] = {
			//D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0
		};
		int featureCount = ARRAYSIZE(features);

		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Width = g_width;
		swapChainDesc.BufferDesc.Height = g_height;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.Windowed = true;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = g_hwnd;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;

		HRESULT hr;
		hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE,
			NULL, D3D11_CREATE_DEVICE_DEBUG, features, featureCount, D3D11_SDK_VERSION,
			&swapChainDesc, &g_swapChain, &g_device, &g_featureLevel, &g_immediateContext
		);
		if (FAILED(hr)) {
			return hr;
		}

		// create render target view
		ID3D11Texture2D* pBackBuffer = NULL;
		hr = g_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		if (FAILED(hr)) {
			return hr;
		}

		hr = g_device->CreateRenderTargetView(pBackBuffer, NULL, &g_renderTargetView);
		if (FAILED(hr)) {
			if (pBackBuffer)
				pBackBuffer->Release();
			return hr;
		}
		pBackBuffer->Release();
		pBackBuffer = NULL;

		g_immediateContext->OMSetRenderTargets(1, &g_renderTargetView, NULL);

		// create depth stencil view
		ID3D11Texture2D* pDepthStencilBuffer = NULL;

		D3D11_TEXTURE2D_DESC depthDesc;
		ZeroMemory(&depthDesc, sizeof(depthDesc));
		depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthDesc.Usage = D3D11_USAGE_DEFAULT;
		depthDesc.Width = g_width;
		depthDesc.Height = g_height;
		depthDesc.MipLevels = 1;
		depthDesc.ArraySize = 1;
		depthDesc.MiscFlags = 0;
		depthDesc.SampleDesc.Quality = 0;
		depthDesc.SampleDesc.Count = 1;
		depthDesc.CPUAccessFlags = 0;
		hr = g_device->CreateTexture2D(&depthDesc, NULL, &pDepthStencilBuffer);
		if (FAILED(hr)) {
			return hr;
		}

		D3D11_DEPTH_STENCIL_VIEW_DESC depthViewDesc;
		ZeroMemory(&depthViewDesc, sizeof(depthViewDesc));
		depthViewDesc.Format = depthDesc.Format;
		depthViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthViewDesc.Texture2D.MipSlice = 0;
		hr = g_device->CreateDepthStencilView(pDepthStencilBuffer, &depthViewDesc, &g_depthStencilView);
		if (FAILED(hr)) {
			if (pDepthStencilBuffer)
				pDepthStencilBuffer->Release();
			return hr;
		}
		pDepthStencilBuffer->Release();
		pDepthStencilBuffer = NULL;

		g_immediateContext->OMSetRenderTargets(1, &g_renderTargetView, g_depthStencilView);

		// setup viewport
		D3D11_VIEWPORT viewPort;
		ZeroMemory(&viewPort, sizeof(viewPort));
		viewPort.Height = g_height;
		viewPort.Width = g_width;
		viewPort.TopLeftX = 0;
		viewPort.TopLeftY = 0;
		viewPort.MinDepth = 0.f;
		viewPort.MaxDepth = 1.f;
		g_immediateContext->RSSetViewports(1, &viewPort);

		return S_OK;
	}

#define _RELEASEPOINTER(x) if(x){x->Release();x=NULL;}else{}
	void ClearDevice() {
		if (g_device) {
			g_device->Release();
			g_device = NULL;
		}
		if (g_immediateContext) {
			g_immediateContext->Release();
			g_immediateContext = NULL;
		}
		if (g_swapChain) {
			g_swapChain->Release();
			g_swapChain = NULL;
		}
		if (g_renderTargetView) {
			g_renderTargetView->Release();
			g_renderTargetView = NULL;
		}
		if (g_depthStencilView) {
			g_depthStencilView->Release();
			g_depthStencilView = NULL;
		}
		ModelManager::Uninit();
	}

	LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		PAINTSTRUCT ps;
		HDC hdc;
		switch (msg) {
		case WM_PAINT:
			hdc = BeginPaint(hwnd, &ps);
			EndPaint(hwnd, &ps);
			break;
		case WM_QUIT:
			DestroyWindow(0);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_KEYDOWN: {
			switch (LOWORD(wParam)) {
			case VK_ESCAPE:
				PostQuitMessage(0);
				break;
			}
		}
						 break;
		}
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	void Render() {
		float c[4] = { 1.f, 0.5f, 0.5f, 1.f };
		g_immediateContext->ClearRenderTargetView(g_renderTargetView, c);
		g_immediateContext->ClearDepthStencilView(g_depthStencilView, D3D11_CLEAR_DEPTH, 1.f, 0.f);

		ModelManager::GetInstance()->Render(g_immediateContext);

		g_swapChain->Present(NULL, NULL);
	}
}
