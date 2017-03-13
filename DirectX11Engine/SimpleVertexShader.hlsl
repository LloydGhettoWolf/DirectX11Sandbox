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
	float4 pos  : POSITION;
	float4 norm : NORMAL;
	float4 tex  : TEXCOORD0;
};

struct PixelInput
{
	float4 pos  : SV_POSITION;
	float4 worldPos : POSITION;
	float4 norm : NORMAL;
	float4 tex  : TEXCOORD0;
};


PixelInput main( VertInput vertIn ) 
{
	PixelInput output;
	output.pos = vertIn.pos;
	output.norm = vertIn.norm;
	output.tex = vertIn.tex;

	output.pos = mul(output.pos, world);
	output.worldPos = output.pos;
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, proj);

	return output;
}