#pragma once

#include <d3d11.h>
#include <xnamath.h>

#include "Shader.h"

class ShaderPool
{
public:
	static HRESULT Init(ID3D11Device* device, ID3D11DeviceContext* context);
	static void Uninit();

	static ShaderPool* GetInstance() { return m_instance; }

private:
	static ShaderPool* m_instance;
};