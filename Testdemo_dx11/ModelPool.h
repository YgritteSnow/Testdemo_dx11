#pragma once

#include <d3d11.h>
#include <xnamath.h>
#include <map>

#include "Model.h"

class ModelPool
{
public:
	static HRESULT Init();
	static void Uninit();
	static ModelPool* GetInstance() { return m_instance; }

public:
	ModelPool();
	~ModelPool();

public:
	Model* GetModel(const char* name);

private:
	static ModelPool* m_instance;

private:
	std::map<const char*, Model*> m_map_model;
};