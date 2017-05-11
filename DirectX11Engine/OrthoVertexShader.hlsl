struct VertIn
{
	float3 pos : POSITION;
	float2 uv : TEXCOORD;
};

struct VertOut
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

cbuffer Matrices
{
	matrix proj;
};

VertOut main(VertIn vIn)
{
	VertOut vOut;
	float4 pos = float4(vIn.pos, 1.0f);
	vOut.pos = mul(pos, proj);
	vOut.uv = vIn.uv;
	return vOut;
}