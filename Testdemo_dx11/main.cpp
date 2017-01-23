#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <tchar.h>

const UINT g_width = 500;
const UINT g_height = 400;

HWND g_hwnd = NULL;
HINSTANCE g_hInst = NULL;
IDXGISwapChain* g_swapChain = NULL;
ID3D11Device* g_device = NULL;
ID3D11DeviceContext* g_immediateContext = NULL;
D3D_FEATURE_LEVEL g_featureLevel;
ID3D11RenderTargetView* g_renderTargetView = NULL;

LRESULT CALLBACK winProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hinst, int nCmdShow);
HRESULT InitDevice();
void SetRenderData();
void Render();
void Destroy();

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	if (FAILED(Init(hInst, nCmdShow))){
		Destroy();
		return E_FAIL;
	}
	if (FAILED(InitDevice())){
		Destroy();
		return E_FAIL;
	}

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT){
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else{
			Render();
		}
	}

	Destroy();
	
	return S_OK;
}

HRESULT Init(HINSTANCE hinst, int nCmdShow){
	WNDCLASSEX wc = {
		sizeof(wc),
		CS_HREDRAW | CS_VREDRAW,
		winProc,
		0, 0,
		hinst,
		NULL, NULL, NULL,
		NULL,
		_T("myclass"),
		NULL
	};
	if (FAILED(RegisterClassEx(&wc))){
		return E_FAIL;
	}

	g_hwnd = CreateWindow(_T("myclass"),
		_T("myclass"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		g_width, g_height,
		NULL, NULL, hinst, NULL);
	if (!g_hwnd){
		return E_FAIL;
	}

	ShowWindow(g_hwnd, nCmdShow);
	return S_OK;
}

HRESULT InitDevice(){
	// create device / context / swapchain
	D3D_FEATURE_LEVEL features[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};
	UINT featureCount = ARRAYSIZE(features);

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.Width = g_width;
	swapChainDesc.BufferDesc.Height = g_height;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = g_hwnd;
	swapChainDesc.Windowed = TRUE;
	//swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.SampleDesc.Count = 1;

	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		features,
		featureCount,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&g_swapChain,
		&g_device,
		&g_featureLevel,
		&g_immediateContext
		))){
		return E_FAIL;
	}

	// create target buffer
	ID3D11Texture2D* pSurface = NULL;
	HRESULT hr;
	hr = g_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pSurface);
	if (FAILED(hr))
		return hr;

	hr = g_device->CreateRenderTargetView(pSurface, NULL, &g_renderTargetView);
	pSurface->Release();
	if (FAILED(hr))
		return hr;

	g_immediateContext->OMSetRenderTargets(1, &g_renderTargetView, NULL);

	// set view port
	D3D11_VIEWPORT viewport;
	viewport.Height = g_height;
	viewport.Width = g_width;
	viewport.MaxDepth = 1.0f;
	viewport.MinDepth = 0.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	g_immediateContext->RSSetViewports(1, &viewport);
}

void Destroy(){
	if (g_immediateContext){
		g_immediateContext->Release();
		g_immediateContext = NULL;
	}
	if (g_device){
		g_device->Release();
		g_device = NULL;
	}
	if (g_swapChain){
		g_swapChain->Release();
		g_swapChain = NULL;
	}
	if (g_renderTargetView){
		g_renderTargetView->Release();
		g_renderTargetView = NULL;
	}
}

void SetRenderData(){

}

void Render(){
	float c[4] = { 1.f, 0.f, 0.f, 1.f };
	g_immediateContext->ClearRenderTargetView(g_renderTargetView, c);
	g_swapChain->Present(0, 0);
}

LRESULT CALLBACK winProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	return DefWindowProc(hwnd, msg, wParam, lParam);
}