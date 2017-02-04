#include "Mesh.h"
#include "Device.h"

extern ID3D11DeviceContext* JJ_TEST_DEMO::g_immediateContext;
extern ID3D11Device* JJ_TEST_DEMO::g_device;

Mesh::Mesh()
	: m_vertexBuffer(NULL)
	, m_indiceBuffer(NULL)
{
}

Mesh::~Mesh() {
	if (m_vertexBuffer) {
		m_vertexBuffer->Release();
		m_vertexBuffer = NULL;
	}
}

HRESULT Mesh::Load() {
	if (FAILED(InitVertices())) {
		return E_FAIL;
	}
	if (FAILED(InitIndices())) {
		return E_FAIL;
	}
	return S_OK;
}

HRESULT Mesh::InitVertices() {
	SimpleVertex vertexes[] = {
		XMFLOAT3(-0.5f, -0.5f, -0.5f),
		XMFLOAT3(0.5f, -0.5f, -0.5f),
		XMFLOAT3(0.5f, 0.5f, -0.5f),
		XMFLOAT3(-0.5f, 0.5f, -0.5f),

		XMFLOAT3(-0.5f, -0.5f, 0.5f),
		XMFLOAT3(0.5f, -0.5f, 0.5f),
		XMFLOAT3(0.5f, 0.5f, 0.5f),
		XMFLOAT3(-0.5f, 0.5f, 0.5f),
	};

	// setup vertex 
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(SimpleVertex) * ARRAYSIZE(vertexes);
	vertexBufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA datablob;
	ZeroMemory(&datablob, sizeof(datablob));
	datablob.pSysMem = vertexes;
	HRESULT hr = JJ_TEST_DEMO::g_device->CreateBuffer(&vertexBufferDesc, &datablob, &m_vertexBuffer);
	if (FAILED(hr)) {
		return hr;
	}

	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	JJ_TEST_DEMO::g_immediateContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	return S_OK;
}

HRESULT Mesh::InitIndices() {
	WORD indices[] = {
		0,1,2,
		0,2,3,

		4,6,5,
		4,7,6,

		0,2,1,
		0,3,2,

		4,5,6,
		4,6,7,
	};

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(WORD) * ARRAYSIZE(indices);
	indexBufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA datablob;
	datablob.pSysMem = indices;
	HRESULT hr = JJ_TEST_DEMO::g_device->CreateBuffer(&indexBufferDesc, &datablob, &m_indiceBuffer);
	if (FAILED(hr)) {
		return hr;
	}

	UINT offset = 0;
	JJ_TEST_DEMO::g_immediateContext->IASetIndexBuffer(m_indiceBuffer, DXGI_FORMAT_R16_UINT, 0);

	JJ_TEST_DEMO::g_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return S_OK;
}

void Mesh::Render() {
	JJ_TEST_DEMO::g_immediateContext->DrawIndexed(24, 0, 0);
}