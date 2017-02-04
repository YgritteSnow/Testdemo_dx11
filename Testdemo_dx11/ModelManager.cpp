#include "ModelManager.h"

#include <algorithm>

#include "ShaderPool.h"

ModelManager* ModelManager::m_instance = NULL;

HRESULT ModelManager::Init(ID3D11Device* device, ID3D11DeviceContext* context) {
	if (!m_instance) {
		m_instance = new ModelManager;
	}
	if (!m_instance) {
		return E_FAIL;
	}

	if (FAILED(ShaderPool::Init(device, context))) {
		return E_FAIL;
	}

	auto m = new Model;
	if (FAILED(m->Load())) {
		return E_FAIL;
	}

	m_instance->m_vec_models.clear();
	m_instance->m_vec_models.push_back(m);

	return S_OK;
}

void ModelManager::Uninit() {
	if (m_instance) {
		delete m_instance;
		m_instance = NULL;
	}
}

ModelManager::~ModelManager() {
	for_each(m_vec_models.begin(), m_vec_models.end(), [&](auto& x) {delete x; });
	m_vec_models.clear();
}

void ModelManager::Render(ID3D11DeviceContext* context) {
	for_each(m_vec_models.begin(), m_vec_models.end(), [&](auto& x) {
		x->Render();
	});
}