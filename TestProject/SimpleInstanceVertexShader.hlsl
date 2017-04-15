//Simple Vert shader
cbuffer Matrices
{
	matrix world;
	matrix view;
	matrix proj;
}

cbuffer Positions
{
	float3 instPositions[1];
};

struct PixelInput
{
	float4 pos  : SV_POSITION;
};

struct VertInput
{
	float3 pos : POSITION;
	float3 offset : INSTANCEPOS;
};

PixelInput main(VertInput vertIn)
{
	PixelInput output;
	output.pos = float4(vertIn.pos + vertIn.offset, 1.0f);
	output.pos = mul(output.pos, world);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, proj);

	return output;
}