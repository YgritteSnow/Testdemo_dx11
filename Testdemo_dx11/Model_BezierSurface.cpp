#include "Model_BezierSurface.h"

Model_BezierSurface::Model_BezierSurface()
: isDirty(false)
{}

Model_BezierSurface::~Model_BezierSurface()
{}

// ������ߵĿ��Ƶ�
void Model_BezierSurface::Clear() {
	m_vec_top.clear();
	m_vec_bottom.clear();
	m_vec_left_noedge.clear();
	m_vec_right_noedge.clear();
	isDirty = true;
}
void Model_BezierSurface::AddTopVertice(VertexType v) {
	m_vec_top.push_back(v);
	isDirty = true;
}
void Model_BezierSurface::AddBottomVertice(VertexType v) {
	m_vec_bottom.push_back(v);
	isDirty = true;
}
void Model_BezierSurface::AddLeftVertice(VertexType v) {
	m_vec_left_noedge.push_back(v);
	isDirty = true;
}
void Model_BezierSurface::AddRightVertice(VertexType v){
	m_vec_right_noedge.push_back(v);
	isDirty = true;
}

// ���¼��������ϵĵ�
void Model_BezierSurface::GenerateCurve(){
	if (!isDirty){
		return;
	}
	

}