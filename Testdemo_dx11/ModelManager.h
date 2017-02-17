#ifndef __MODELMANAGER_H__
#define __MODELMANAGER_H__

#include <d3d11.h>
#include <d3dx11.h>
#include <xnamath.h>
#include <vector>

#include "Model.h"

/************************************************************************/
/* initialize device about model(vertex)
/************************************************************************/
class ModelManager{
public:
	static HRESULT Init(ID3D11Device* device, ID3D11DeviceContext* context);
	static void Uninit();
	static ModelManager* GetInstance() {return m_instance;}
private:
	~ModelManager();
public:
	void Render(ID3D11DeviceContext* context);

private:
	std::vector<Model*> m_vec_models;

private:
	static ModelManager* m_instance;
};
#endif