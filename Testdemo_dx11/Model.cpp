#include "Model.h"
#include "Utilities.h"

Model::Model()
	: m_mesh(NULL)
{
}

Model::~Model() {
	Utilities::ReleasePointer(m_mesh);
}

HRESULT Model::Load() {
	m_mesh = new Mesh;
	if (!m_mesh) {
		return E_FAIL;
	}
	if (FAILED(m_mesh->Load())) {
		return E_FAIL;
	}

	m_shader = new Shader;
	if (!m_shader) {
		return E_FAIL;
	}
	if (FAILED(m_shader->Load())) {
		return E_FAIL;
	}
	return S_OK;
}

void Model::Render() {
	m_shader->Render();
	m_mesh->Render();
}