#include "Model_BezierSurface.h"

Model_BezierSurface::Model_BezierSurface()
: isDirty(false)
{}

Model_BezierSurface::~Model_BezierSurface()
{}

// ������ߵĿ��Ƶ�
void Model_BezierSurface::AddControlPoint(VertexType v){
	m_vec_meshVertices.push_back(v);
	isDirty = true;
}

// ���¼��������ϵĵ�
void Model_BezierSurface::GenerateCurve(){
	if (!isDirty){
		return;
	}
	//
}