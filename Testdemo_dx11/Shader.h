#pragma once

#include <d3d11.h>
#include <string>

class Shader
{
public:
	Shader();
	~Shader();

	HRESULT Load(const char* name);
	void Render();

private:
	HRESULT InitShaders();
	HRESULT InitVertexShader();
	HRESULT InitPixelShader();
	HRESULT InitShaderBuffers();
private:
	std::string name;
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_inputLayout;
	ID3D11Buffer* m_constantBuffer;
};
