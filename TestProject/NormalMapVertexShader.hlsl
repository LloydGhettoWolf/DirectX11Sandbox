//Normal map shader
cbuffer Matrices
{
	matrix world;
	matrix view;
	matrix proj;
}

//typedefs
struct VertInput
{
	float3 pos  : POSITION;
	float3 norm : NORMAL;
	float2 tex  : TEXCOORD0;
	float3 tangent: TANGENT;
};

struct PixelInput
{
	float4 pos  : SV_POSITION;
	float4 worldPos : POSITION;
	float3 norm : NORMAL;
	float2 tex  : TEXCOORD0;
	float3 tangent: TANGENT;
};


PixelInput main(VertInput vertIn)
{
	PixelInput output;
	output.pos = float4(vertIn.pos, 1.0f);
	output.norm = vertIn.norm;
	output.tex = vertIn.tex;
	output.tex.y = -output.tex.y;
	output.tangent = vertIn.tangent;

	output.pos = mul(output.pos, world);
	output.worldPos = output.pos;
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, proj);

	return output;
}