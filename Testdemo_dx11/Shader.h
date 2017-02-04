#pragma once

#include <d3d11.h>
#include <xnamath.h>
#include "Device.h"

class Shader
{
public:
	Shader();
	~Shader();

	HRESULT Load();
	void Render();
};

class ShaderManager
{
public:
	static HRESULT Init(ID3D11Device* device, ID3D11DeviceContext* context);
	static void Uninit();

	static ShaderManager* GetInstance() { return m_instance; }
public:
	void Render(ID3D11DeviceContext* context);

private:
	ShaderManager() :m_vertexShader(NULL), m_pixelShader(NULL), m_inputLayout(NULL), m_constantBuffer(NULL) {}
	~ShaderManager();

private:
	HRESULT InitShaders(ID3D11Device* device, ID3D11DeviceContext* context);
	HRESULT InitVertexShader(ID3D11Device* device, ID3D11DeviceContext* context);
	HRESULT InitPixelShader(ID3D11Device* device, ID3D11DeviceContext* context);
	HRESULT InitShaderBuffers(ID3D11Device* device, ID3D11DeviceContext* context);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_inputLayout;
	ID3D11Buffer* m_constantBuffer;
private:
	static ShaderManager* m_instance;
};