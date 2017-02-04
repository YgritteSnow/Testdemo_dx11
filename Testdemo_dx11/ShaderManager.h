#pragma once

#include <d3d11.h>
#include <xnamath.h>

#include "Shader.h"

class ShaderManager
{
public:
	static HRESULT Init(ID3D11Device* device, ID3D11DeviceContext* context);
	static void Uninit();

	static ShaderManager* GetInstance() { return m_instance; }
public:
	void Render(ID3D11DeviceContext* context);

private:
	Shader* m_cur_shader;
private:
	static ShaderManager* m_instance;
};