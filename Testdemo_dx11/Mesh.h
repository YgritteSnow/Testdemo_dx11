#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include <xnamath.h>

/************************************************************************/
/* A vertex struct which contains only position
/************************************************************************/
struct SimpleVertex {
	XMFLOAT3 pos;
};

/************************************************************************/
/* initialize device about model(vertex)
/************************************************************************/
class Mesh {
public:
	Mesh();
	~Mesh();

	HRESULT Load();
	void Render();

private:
	HRESULT InitVertices();
	HRESULT InitIndices();

private:
	ID3D11Buffer* m_vertexBuffer = NULL;
	ID3D11Buffer* m_indiceBuffer = NULL;
};