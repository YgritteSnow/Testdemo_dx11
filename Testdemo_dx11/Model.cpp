#include "Model.h"
#include "Device.h"

extern ID3D11DeviceContext* JJ_TEST_DEMO::g_immediateContext;
extern ID3D11Device* JJ_TEST_DEMO::g_device;

Model::Model()
	: m_mesh(nullptr)
{}

void Model::RealRender()
{}