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
};

struct PixelInput
{
	float4 pos  : SV_POSITION;
	float3 viewPos : POSITION;
	float3 norm : NORMAL;
};

PixelInput main( VertInput vertIn ) 
{
	PixelInput output;
	output.pos = float4(vertIn.pos, 1.0f);

	output.pos = mul(output.pos, world);
	output.norm = mul(vertIn.norm, world);
	output.pos = mul(output.pos, view);
	output.viewPos = output.pos;
	output.pos = mul(output.pos, proj);

	return output;
}