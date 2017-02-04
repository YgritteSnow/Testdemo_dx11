#pragma once

#include <d3d11.h>

class Shader
{
public:
	Shader();
	~Shader();

	HRESULT Load();
	void Render();

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
};
