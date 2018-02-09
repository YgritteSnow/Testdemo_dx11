#pragma once
#include <DirectXMath.h>
#include <vector>

#include "Model.h"

struct QuadVertex{
	DirectX::XMFLOAT3 pos;
};

class Model_Quad : public Model
{
public:
	typedef QuadVertex VertexType;
	Model_Quad();
	~Model_Quad();

public:

private:
};