//Simple Vert shader
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
	float2 tex  : TEXCOORD;
};

struct PixelInput
{
	float4 pos  : SV_POSITION;
	float3 worldPos : POSITION;
	float3 norm : NORMAL;
	float2 tex  : TEXCOORD;
};

PixelInput main( VertInput vertIn ) 
{
	PixelInput output;
	output.pos = float4(vertIn.pos, 1.0f);
	output.norm = mul(vertIn.norm, world);

	output.pos = mul(output.pos, world);
	output.worldPos = output.pos;
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, proj);

	output.tex = vertIn.tex;


	return output;
}