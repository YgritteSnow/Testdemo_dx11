#include "Shader.h"

#include <d3dx11.h>
#include <d3dcompiler.h>
#include <tchar.h>
#include "Utilities.h"
#include "Device.h"

ShaderManager* ShaderManager::m_instance = NULL;

HRESULT ShaderManager::Init(ID3D11Device* device, ID3D11DeviceContext* context) {
	m_instance = new ShaderManager;
	if (!m_instance) {
		return E_FAIL;
	}
	if (FAILED(m_instance->InitShaders(device, context))) {
		return E_FAIL;
	}
	return S_OK;
}

void ShaderManager::Uninit() {
	if (m_instance) {
		delete m_instance;
		m_instance = NULL;
	}
}

ShaderManager::~ShaderManager() {
	Utilities::ReleaseCom(m_vertexShader);
	Utilities::ReleaseCom(m_pixelShader);
	Utilities::ReleaseCom(m_inputLayout);
}

HRESULT ShaderManager::InitShaders(ID3D11Device* device, ID3D11DeviceContext* context) {
	if (FAILED(InitVertexShader(device, context))) {
		return E_FAIL;
	}
	if (FAILED(InitPixelShader(device, context))) {
		return E_FAIL;
	}
	if (FAILED(InitShaderBuffers(device, context))) {
		return E_FAIL;
	}
	return S_OK;
}

HRESULT ShaderManager::InitVertexShader(ID3D11Device* device, ID3D11DeviceContext* context) {
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

	if (FAILED(hr)) {
		if (logblob) {
			OutputDebugStringA((char*)logblob->GetBufferPointer());
			logblob->Release();
		}
		if (vsBlob) {
			vsBlob->Release();
		}
		return hr;
	}
	if (logblob) {
		logblob->Release();
	}

	hr = device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(hr)) {
		vsBlob->Release();
		return hr;
	}

	// setup input layout
	D3D11_INPUT_ELEMENT_DESC inputLayout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	hr = device->CreateInputLayout(inputLayout, ARRAYSIZE(inputLayout),
		vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &m_inputLayout);
	vsBlob->Release();
	if (FAILED(hr)) {
		return hr;
	}
	context->IASetInputLayout(m_inputLayout);

	return S_OK;
}

HRESULT ShaderManager::InitPixelShader(ID3D11Device* device, ID3D11DeviceContext* context) {
	ID3DBlob* psBlob = NULL;
	ID3DBlob* logBlob = NULL;
	HRESULT hr = D3DX11CompileFromFile(_T("test.fx"), NULL, NULL,
		"PS_Main", "ps_4_0",
		D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG, 0,
		NULL,
		&psBlob, &logBlob, NULL);
	if (FAILED(hr)) {
		if (logBlob) {
			OutputDebugStringA((char*)logBlob->GetBufferPointer());
			logBlob->Release();
		}
		if (psBlob) {
			psBlob->Release();
		}
		return hr;
	}
	if (logBlob) {
		logBlob->Release();
	}

	hr = device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), NULL, &m_pixelShader);
	psBlob->Release();
	if (FAILED(hr)) {
		return hr;
	}

	return S_OK;
}

struct ModelConstantBuffer {
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;
};
HRESULT ShaderManager::InitShaderBuffers(ID3D11Device* device, ID3D11DeviceContext* context) {
	D3D11_BUFFER_DESC cd;
	ZeroMemory(&cd, sizeof(cd));
	cd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cd.ByteWidth = sizeof(ModelConstantBuffer);
	cd.CPUAccessFlags = 0;
	cd.Usage = D3D11_USAGE_DEFAULT;
	HRESULT hr = device->CreateBuffer(&cd, NULL, &m_constantBuffer);
	return hr;
}

void ShaderManager::Render(ID3D11DeviceContext* context) {
	ModelConstantBuffer cb;
	ZeroMemory(&cb, sizeof(cb));
	cb.world = XMMatrixIdentity();
	cb.view = XMMatrixTranspose(XMMatrixLookAtLH(
		XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f),
		XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)));
	cb.projection = XMMatrixTranspose(XMMatrixPerspectiveFovLH(XM_PIDIV2, float(JJ_TEST_DEMO::g_width) / JJ_TEST_DEMO::g_height, 0.1f, 100.f));

	context->UpdateSubresource(m_constantBuffer, 0, NULL, &cb, 0, 0);

	context->VSSetConstantBuffers(0, 1, &m_constantBuffer);
	context->VSSetShader(m_vertexShader, NULL, 0);
	context->PSSetShader(m_pixelShader, NULL, 0);
}