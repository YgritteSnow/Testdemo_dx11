#include "ModelPool.h"

#include <algorithm>

/************************************************************************/
/*                                                                      */
/************************************************************************/
ModelPool* ModelPool::m_instance = NULL;

ModelPool::ModelPool()
{
	m_map_model.clear();
}

ModelPool::~ModelPool()
{
	//for_each(m_map_model.begin(), m_map_model.end(), [&](auto& x) {delete x.second; });
}

HRESULT ModelPool::Init() {
	m_instance = new ModelPool;
	if (!m_instance) {
		return E_FAIL;
	}
	return S_OK;
}

void ModelPool::Uninit() {
	if (m_instance) {
		delete m_instance;
		m_instance = NULL;
	}
}

Model* ModelPool::GetModel(const char* name)
{
	Model* res = nullptr;
	auto find_res = m_map_model.find(name);
	if (find_res == m_map_model.end())
	{
		res = new Model;
		if (FAILED(res->Load(name)))
		{
			delete res;
			return nullptr;
		}
		m_map_model[name] = res;
	}
	else
	{
		res = find_res->second;
	}
	return res;
}