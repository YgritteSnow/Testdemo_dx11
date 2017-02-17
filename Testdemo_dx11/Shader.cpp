#include "Shader.h"

#include <d3dx11.h>
#include <xnamath.h>
#include <d3dcompiler.h>
#include <tchar.h>

#include "Utilities.h"
#include "Device.h"

extern ID3D11Device* JJ_TEST_DEMO::g_device;
extern ID3D11DeviceContext* JJ_TEST_DEMO::g_immediateContext;
/************************************************************************/
/*                                                                      */
/************************************************************************/
Shader::Shader()
	:m_vertexShader(NULL)
	, m_pixelShader(NULL)
	, m_inputLayout(NULL)
	, m_constantBuffer(NULL) 
{}

Shader::~Shader() {
	Utilities::ReleaseCom(m_vertexShader);
	Utilities::ReleaseCom(m_pixelShader);
	Utilities::ReleaseCom(m_inputLayout);
}

HRESULT Shader::Load() {
	return InitShaders(JJ_TEST_DEMO::g_device, JJ_TEST_DEMO::g_immediateContext);
}

struct ModelConstantBuffer {
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;
};

void Shader::Render() {
	ModelConstantBuffer cb;
	ZeroMemory(&cb, sizeof(cb));
	cb.world = XMMatrixIdentity();
	cb.view = XMMatrixTranspose(XMMatrixLookAtLH(
		XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f),
		XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)));
	cb.projection = XMMatrixTranspose(XMMatrixPerspectiveFovLH(XM_PIDIV2, float(JJ_TEST_DEMO::g_width) / JJ_TEST_DEMO::g_height, 0.1f, 100.f));

	JJ_TEST_DEMO::g_immediateContext->UpdateSubresource(m_constantBuffer, 0, NULL, &cb, 0, 0);

	JJ_TEST_DEMO::g_immediateContext->VSSetConstantBuffers(0, 1, &m_constantBuffer);
	JJ_TEST_DEMO::g_immediateContext->VSSetShader(m_vertexShader, NULL, 0);
	JJ_TEST_DEMO::g_immediateContext->PSSetShader(m_pixelShader, NULL, 0);
}

HRESULT Shader::InitShaders(ID3D11Device* device, ID3D11DeviceContext* context) {
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

HRESULT Shader::InitVertexShader(ID3D11Device* device, ID3D11DeviceContext* context) {
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

HRESULT Shader::InitPixelShader(ID3D11Device* device, ID3D11DeviceContext* context) {
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

HRESULT Shader::InitShaderBuffers(ID3D11Device* device, ID3D11DeviceContext* context) {
	D3D11_BUFFER_DESC cd;
	ZeroMemory(&cd, sizeof(cd));
	cd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cd.ByteWidth = sizeof(ModelConstantBuffer);
	cd.CPUAccessFlags = 0;
	cd.Usage = D3D11_USAGE_DEFAULT;
	HRESULT hr = device->CreateBuffer(&cd, NULL, &m_constantBuffer);
	return hr;
}
