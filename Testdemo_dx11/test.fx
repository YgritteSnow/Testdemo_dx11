cbuffer ModelConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
}

struct VS_OUT {
	float4 pos : SV_POSITION;
	float4 color : COLOR0;
};

VS_OUT VS_Main(float4 pos: POSITION)
{
	VS_OUT output = (VS_OUT)0;
	output.pos = mul(pos, World);
	output.pos = mul(output.pos, View);
	output.pos = mul(output.pos, Projection);
	//return float4(pos.xy, 1.f, 1.f);
	output.color = float4(float3(World._m00, World._m11, World._m22)/2, 1.f);
	return output;
}

float4 PS_Main(VS_OUT input) : SV_Target
{
#ifdef FFF
	return float4(0.f, 1.f, 0.f, 1.f);
#else
	return float4(1.f, 0.f, 0.f, 1.f);
#endif
	//return input.color;
}