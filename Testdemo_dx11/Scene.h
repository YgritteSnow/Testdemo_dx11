#ifndef __SCENE_H__
#define __SCENE_H__

#include <d3d11.h>
#include <d3dx11.h>
#include <xnamath.h>
#include <vector>

#include "Model.h"

/************************************************************************/
/* initialize device about model(vertex)
/************************************************************************/
class Scene{
private:
	Scene() {};
	~Scene();

public:
	static HRESULT Init(ID3D11Device* device, ID3D11DeviceContext* context);
	static void Uninit();
	static Scene* GetInstance() {return m_instance;}
public:
	void AddModel(Model* model);
	void RemoveModel(Model* model);
	void Render();

private:
	std::vector<Model*> m_vec_models;

private:
	static Scene* m_instance;
};
#endif