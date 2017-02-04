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
	return m_mesh->Load();
}

void Model::Render() {
	m_mesh->Render();
}