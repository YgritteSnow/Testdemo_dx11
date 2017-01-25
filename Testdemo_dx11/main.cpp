#include <d3d11.h>
#include <d3dx11.h>
#include <Windows.h>
#include <tchar.h>
#include <xnamath.h>
#include <d3dcompiler.h>
#include "Model.h"

const int g_width = 500;
const int g_height = 300;

ID3D11DeviceContext* g_immediateContext = NULL;
ID3D11Device* g_device = NULL;
IDXGISwapChain* g_swapChain = NULL;
D3D_FEATURE_LEVEL g_featureLevel;
ID3D11RenderTargetView* g_renderTargetView = NULL;

ID3D11Buffer* g_vertexBuffer = NULL;
//ID3D11Buffer* g_indexBuffer = NULL;
struct SimpleVertex{
	XMFLOAT3 pos;
};

ID3D11VertexShader* g_vertexShader = NULL;
ID3D11InputLayout* g_inputLayout = NULL;
ID3D11PixelShader* g_pixelShader = NULL;

HWND g_hwnd = NULL;
HINSTANCE g_instance = NULL;

LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
HRESULT InitWindow(HINSTANCE hwnd, int nCmdShow);
HRESULT InitDevice();
HRESULT InitVertexes();
HRESULT InitShaders();
HRESULT InitVertexShader();
HRESULT InitPixelShader();
void ClearDevice();
void Render();

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd){
	if (FAILED(InitWindow(hInstance, nShowCmd)))
		return 0;
	if (FAILED(InitDevice())){
		ClearDevice();
		return 0;
	}
	if (FAILED(InitShaders())) {
		ClearDevice();
		return 0;
	}
	if (FAILED(InitVertexes())){
		ClearDevice();
		return 0;
	}

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (WM_QUIT != msg.message){
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else{
			Render();
		}
	}
	return 0;
}

HRESULT InitWindow(HINSTANCE hinst, int nCmdShow){
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

HRESULT InitDevice(){
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
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 60;
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
	if (FAILED(hr)){
		return hr;
	}

	// create render target view
	ID3D11Texture2D* pBackBuffer = NULL;
	hr = g_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr)){
		if (pBackBuffer)
			pBackBuffer->Release();
		return hr;
	}

	hr = g_device->CreateRenderTargetView(pBackBuffer, NULL, &g_renderTargetView);
	if (FAILED(hr)){
		if (pBackBuffer)
			pBackBuffer->Release();
		return hr;
	}
	pBackBuffer->Release();
	pBackBuffer = NULL;

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
HRESULT InitVertexes(){
	// setup vertex 
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(SimpleVertex)* 3;
	vertexBufferDesc.CPUAccessFlags = 0;

	SimpleVertex vertexes[] = {
		XMFLOAT3(0.0f, 0.5f, 0.5f),
		XMFLOAT3(0.5f, -0.5f, 0.5f),
		XMFLOAT3(-0.5f, -0.5f, 0.5f),
	};
	D3D11_SUBRESOURCE_DATA datablob;
	ZeroMemory(&datablob, sizeof(datablob));
	datablob.pSysMem = vertexes;
	HRESULT hr = g_device->CreateBuffer(&vertexBufferDesc, &datablob, &g_vertexBuffer);
	if (FAILED(hr)){
		return hr;
	}

	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	g_immediateContext->IASetVertexBuffers(0, 1, &g_vertexBuffer, &stride, &offset);

	g_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return S_OK;
}

HRESULT InitShaders(){
	if (FAILED(InitVertexShader())){
		return E_FAIL;
	}
	if (FAILED(InitPixelShader())){
		return E_FAIL;
	}
	return S_OK;
}

HRESULT InitVertexShader(){
	ID3DBlob* vsBlob;
	ID3DBlob* logblob;
	HRESULT hr = E_FAIL;
	hr = D3DX11CompileFromFile(_T("test.fx"), NULL, NULL,
		"VS_Main", "vs_4_0",
		D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG,
		0, NULL,
		&vsBlob,
		&logblob,
		NULL
		);

	if (FAILED(hr)){
		if (logblob){
			OutputDebugStringA((char*)logblob->GetBufferPointer());
			logblob->Release();
		}
		if (vsBlob){
			vsBlob->Release();
		}
		return hr;
	}
	if (logblob){
		logblob->Release();
	}

	hr = g_device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), NULL, &g_vertexShader);
	if (FAILED(hr)){
		vsBlob->Release();
		return hr;
	}

	// setup input layout
	D3D11_INPUT_ELEMENT_DESC inputLayout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	hr = g_device->CreateInputLayout(inputLayout, ARRAYSIZE(inputLayout),
		vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &g_inputLayout);
	vsBlob->Release();
	if (FAILED(hr)) {
		return hr;
	}
	g_immediateContext->IASetInputLayout(g_inputLayout);

	return S_OK;
}

HRESULT InitPixelShader(){
	ID3DBlob* psBlob = NULL;
	ID3DBlob* logBlob = NULL;
	HRESULT hr = D3DX11CompileFromFile(_T("test.fx"), NULL, NULL,
		"PS_Main", "ps_4_0",
		D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG, 0,
		NULL,
		&psBlob, &logBlob, NULL);
	if (FAILED(hr)){
		if (logBlob){
			OutputDebugStringA((char*)logBlob->GetBufferPointer());
			logBlob->Release();
		}
		if (psBlob){
			psBlob->Release();
		}
		return hr;
	}
	if (logBlob){
		logBlob->Release();
	}

	hr = g_device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), NULL, &g_pixelShader);
	psBlob->Release();
	if (FAILED(hr)){
		return hr;
	}

	return S_OK;
}

#define _RELEASEPOINTER(x) if(x){x->Release();x=NULL;}else{}
void ClearDevice(){
	if (g_device){
		g_device->Release();
		g_device = NULL;
	}
	if (g_immediateContext){
		g_immediateContext->Release();
		g_immediateContext = NULL;
	}
	if (g_swapChain){
		g_swapChain->Release();
		g_swapChain = NULL;
	}
	if (g_renderTargetView){
		g_renderTargetView->Release();
		g_renderTargetView = NULL;
	}
	_RELEASEPOINTER(g_vertexBuffer);
	_RELEASEPOINTER(g_vertexShader);
	_RELEASEPOINTER(g_pixelShader);
	_RELEASEPOINTER(g_inputLayout);
}

LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
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
	case WM_KEYDOWN:{
						switch (LOWORD(wParam)){
						case VK_ESCAPE:
							PostQuitMessage(0);
							break;
						}
	}
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void Render(){
	float c[4] = { 1.f, 0.5f, 0.5f, 1.f };
	g_immediateContext->ClearRenderTargetView(g_renderTargetView, c);

	g_immediateContext->VSSetShader(g_vertexShader, NULL, 0);
	g_immediateContext->PSSetShader(g_pixelShader, NULL, 0);

	g_immediateContext->Draw(3, 0);

	g_swapChain->Present(NULL, NULL);
}