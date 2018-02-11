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
	Mesh(const char* filename);
	~Mesh();

	HRESULT Load(const char* filename);
	void Render();

private:
	HRESULT InitMesh(const char* filename);

	HRESULT InitDefaultMesh();
	HRESULT InitDefaultVertices();
	HRESULT InitDefaultIndices();

private:
	ID3D11Buffer* m_vertexBuffer = NULL;
	ID3D11Buffer* m_indiceBuffer = NULL;
};