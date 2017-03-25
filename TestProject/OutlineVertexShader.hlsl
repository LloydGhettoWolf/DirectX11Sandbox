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
};

struct PixelInput
{
	float4 pos  : SV_POSITION;
};

PixelInput main(VertInput vertIn)
{
	PixelInput output;
	output.pos = vertIn.pos;
	output.pos = mul(output.pos, world);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, proj);

	return output;
}