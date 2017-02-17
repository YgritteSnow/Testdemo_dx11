#include "Model_BezierSurface.h"

Model_BezierSurface::Model_BezierSurface()
: isDirty(false)
{}

Model_BezierSurface::~Model_BezierSurface()
{}

// 添加曲线的控制点
void Model_BezierSurface::AddControlPoint(VertexType v){
	m_vec_meshVertices.push_back(v);
	isDirty = true;
}

// 重新计算曲面上的点
void Model_BezierSurface::GenerateCurve(){
	if (!isDirty){
		return;
	}
	//
}