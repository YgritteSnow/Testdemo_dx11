#pragma once
#include <DirectXMath.h>
#include <vector>

#include "Model.h"

struct Vertex{
	DirectX::XMFLOAT3 pos;
};

class Model_BezierSurface : public Model
{
public:
	typedef Vertex VertexType;
	Model_BezierSurface(){}
	~Model_BezierSurface(){}
public:
	void OnLoad();
	void OnDestroy();
	void GetVertices();
	void GetIndices();
private:
	void AddControlPoint(VertexType v);
	void GenerateCurve();
private:
	bool isDirty;
	typedef std::vector<VertexType> VertexListType;
	// 曲面的控制点
	std::vector<VertexListType> m_vec_controlPoints;
	// 生成的mesh的顶点
	std::vector<VertexType> m_vec_meshVertices;
	std::vector<DWORD> m_vec_Indices;
};