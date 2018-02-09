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
	Model_BezierSurface();
	~Model_BezierSurface();

public:

private:
	void Clear();
	void AddTopVertice(VertexType v);
	void AddBottomVertice(VertexType v);
	void AddLeftVertice(VertexType v);
	void AddRightVertice(VertexType v);
	void GenerateCurve();
private:
	bool isDirty;
	typedef std::vector<VertexType> VertexListType;
	// 曲面的控制点
	// *-*-*-* // col_num = 4, row_num = 3
	// |     |
	// *     *
	// |     |
	// *-*-*-*
	VertexListType m_vec_top;
	VertexListType m_vec_bottom;
	VertexListType m_vec_left_noedge;
	VertexListType m_vec_right_noedge;
	// 生成的mesh的顶点
	std::vector<VertexType> m_vec_meshVertices;
	std::vector<DWORD> m_vec_Indices;
};