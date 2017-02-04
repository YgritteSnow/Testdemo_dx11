#include "ShaderPool.h"

/************************************************************************/
/*                                                                      */
/************************************************************************/
ShaderPool* ShaderPool::m_instance = NULL;

HRESULT ShaderPool::Init(ID3D11Device* device, ID3D11DeviceContext* context) {
	m_instance = new ShaderPool;
	if (!m_instance) {
		return E_FAIL;
	}
	return S_OK;
}

void ShaderPool::Uninit() {
	if (m_instance) {
		delete m_instance;
		m_instance = NULL;
	}
}