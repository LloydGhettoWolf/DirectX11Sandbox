cbuffer Matrices
{
	matrix proj;
};

float4 main( float4 pos : POSITION ) : SV_POSITION
{
	pos = mul(pos, proj);
	return pos;
}