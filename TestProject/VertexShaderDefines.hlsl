
//Simple Vert shader
cbuffer Matrices
{
	matrix world;
	matrix view;
	matrix proj;
}

#ifdef STANDARD_VERTEX_INPUT

struct StandardVertexInput
{
	float3 pos  : POSITION;
	float3 norm : NORMAL;
	float2 tex  : TEXCOORD0;
};


struct StandardPixelInput
{
	float4 pos  : SV_POSITION;
	float4 worldPos : POSITION;
	float3 norm : NORMAL;
	float2 tex  : TEXCOORD0;
};
#endif

#ifdef NORM_MAP_VERTEX_INPUT

struct NormMapVertexInput
{
	float3 pos  : POSITION;
	float3 norm : NORMAL;
	float2 tex  : TEXCOORD0;
	float3 tangent: TANGENT;
};

struct NormMapPixelInput
{
	float4 pos  : SV_POSITION;
	float4 worldPos : POSITION;
	float3 norm : NORMAL;
	float2 tex  : TEXCOORD0;
	float3 tangent: TANGENT;
};

#endif

#ifdef STANDARD_VERTEX_INPUT

StandardPixelInput GenerateInput(StandardVertexInput vertIn)
{
	StandardPixelInput output;
#endif

#ifdef NORM_MAP_VERTEX_INPUT
NormMapPixelInput GenerateInput(NormMapVertexInput vertIn)
{
	NormMapPixelInput output;
	output.tangent = vertIn.tangent;
#endif

	output.pos = float4(vertIn.pos, 1.0f);
	output.norm = vertIn.norm;
	output.tex = vertIn.tex;
	output.tex.y = -output.tex.y;

	output.pos = mul(output.pos, world);
	output.worldPos = output.pos;
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, proj);

	return output;
}