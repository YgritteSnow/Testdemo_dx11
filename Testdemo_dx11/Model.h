#ifndef __MODEL_H__
#define __MODEL_H__

#include <d3d11.h>
#include <d3dx11.h>

#include "Mesh.h"
#include "ShaderPool.h"

class Model
{
public:
	Model();
	~Model() {};

public:
	bool Load(const char* filename) {
		m_mesh = new Mesh();
		if (m_mesh == nullptr)
			return false;

		m_shader = ShaderPool::GetInstance()->GetShader("test.fx");
		if (m_shader == nullptr)
			return false;

		OnLoad();
		return true;
	}
	void Render() {
		BeforeRender();
		RealRender();
		AfterRender();
	}
	void Destroy() {
		OnDestroy();
		delete m_mesh;
	}

protected:
	virtual void OnLoad() {};
	virtual void BeforeRender() {};
	virtual void AfterRender() {};
	virtual void OnDestroy() {};

private:
	void RealRender();

protected:
	Mesh* m_mesh;
	Shader* m_shader;
};

#endif