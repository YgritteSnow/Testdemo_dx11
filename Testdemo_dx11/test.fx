float4 VS_Main(float4 pos: POSITION) : SV_POSITION
{
	return pos;
}

float4 PS_Main(float4 Pos : SV_POSITION) : SV_Target
{
	return float4(0.0f, 0.0f, 1.0f, 1.0f);
}