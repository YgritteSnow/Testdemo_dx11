#include "Scene.h"

#include <algorithm>
#include "ShaderPool.h"

Scene* Scene::m_instance = NULL;

HRESULT Scene::Init(ID3D11Device* device, ID3D11DeviceContext* context) {
	if (!m_instance) {
		m_instance = new Scene;
	}
	if (!m_instance) {
		return E_FAIL;
	}

	if (FAILED(ShaderPool::Init(device, context))) {
		return E_FAIL;
	}

	auto m = new Model;
	if (FAILED(m->Load(""))) {
		return E_FAIL;
	}

	m_instance->m_vec_models.clear();
	m_instance->m_vec_models.push_back(m);

	return S_OK;
}

void Scene::Uninit() {
	if (m_instance) {
		delete m_instance;
		m_instance = NULL;
	}
}

Scene::~Scene() {
	for_each(m_vec_models.begin(), m_vec_models.end(), [&](auto& x) {delete x; });
	m_vec_models.clear();
}

void Scene::Render() {
	for_each(m_vec_models.begin(), m_vec_models.end(), [&](auto& x) {
		x->Render();
	});
}