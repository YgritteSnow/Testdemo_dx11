#include "Scene.h"

#include <algorithm>

Scene* Scene::m_instance = NULL;

HRESULT Scene::Init(ID3D11Device* device, ID3D11DeviceContext* context) {
	if (!m_instance) {
		m_instance = new Scene;
	}
	if (!m_instance) {
		return E_FAIL;
	}

	m_instance->m_vec_models.clear();

	return S_OK;
}

void Scene::Uninit() {
	if (m_instance) {
		delete m_instance;
		m_instance = NULL;
	}
}

void Scene::AddModel(Model* model)
{
	m_instance->m_vec_models.push_back(model);
}
void Scene::RemoveModel(Model* model)
{
	m_instance->m_vec_models.erase(std::remove_if(m_instance->m_vec_models.begin(), m_instance->m_vec_models.end(),
		[&model](Model*& m) {return m == model; }));
}

Scene::~Scene() {
	m_vec_models.clear();
}

void Scene::Render() {
	for_each(m_vec_models.begin(), m_vec_models.end(), [&](auto& x) {
		x->Render();
	});
}