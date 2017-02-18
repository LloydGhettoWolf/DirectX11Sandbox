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
	float4 pos : POSITION;
	float4 col : COLOR;
};

struct PixelInput
{
	float4 pos : SV_POSITION;
	float4 col : COLOR;
};

PixelInput main( VertInput vertIn ) 
{
	PixelInput output;
	vertIn.pos.w = 1.0f;

	output.pos = mul(vertIn.pos, world);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, proj);

	output.col = vertIn.col;

	return output;
}