#ifndef __MODEL_H__
#define __MODEL_H__

#include <d3d11.h>
#include <d3dx11.h>

#include "VertexBuffer.h"

class Model
{
public:
	Model();
private:
	VertexBuffer* pVB;
};

class ModelManager
{
public:
};
#endif