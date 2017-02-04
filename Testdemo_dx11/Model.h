#pragma once

#include "Mesh.h"
#include "Shader.h"

class Model {
public:
	Model();
	~Model();

	HRESULT Load();
	void Render();

private:
	Mesh* m_mesh;
};