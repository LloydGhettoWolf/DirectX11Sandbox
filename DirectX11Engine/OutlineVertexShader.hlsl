cbuffer Matrices
{
	matrix world;
	matrix view;
	matrix proj;
}

//typedefs

float4 main( float4 pos : POSITION )
{
	pos = mul(pos, world);
	pos = mul(pos, view);
	pos = mul(pos, proj);

	return pos;
}