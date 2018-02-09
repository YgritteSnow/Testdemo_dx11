#pragma once

#include <d3d11.h>
#include <xnamath.h>
#include <map>

#include "Shader.h"

class ShaderPool
{
public:
	static HRESULT Init();
	static void Uninit();
	static ShaderPool* GetInstance() { return m_instance; }

public:
	Shader* GetShader(const char* name);

private:
	static ShaderPool* m_instance;

private:
	std::map<std::string, Shader> m_vec_shader;
};