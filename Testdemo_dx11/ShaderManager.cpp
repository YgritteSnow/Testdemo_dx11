#include "ShaderManager.h"

/************************************************************************/
/*                                                                      */
/************************************************************************/
ShaderManager* ShaderManager::m_instance = NULL;

HRESULT ShaderManager::Init(ID3D11Device* device, ID3D11DeviceContext* context) {
	m_instance = new ShaderManager;
	if (!m_instance) {
		return E_FAIL;
	}

	// todo
	m_instance->m_cur_shader = new Shader;
	if (!m_instance->m_cur_shader) {
		return E_FAIL;
	}
	if (FAILED(m_instance->m_cur_shader->Load())) {
		return E_FAIL;
	}
	return S_OK;
}

void ShaderManager::Uninit() {
	// todo
	if (m_instance && m_instance->m_cur_shader) {
		delete m_instance->m_cur_shader;
		m_instance->m_cur_shader = NULL;
	}

	if (m_instance) {
		delete m_instance;
		m_instance = NULL;
	}
}

void ShaderManager::Render(ID3D11DeviceContext* context) {
	m_instance->m_cur_shader->Render();
}