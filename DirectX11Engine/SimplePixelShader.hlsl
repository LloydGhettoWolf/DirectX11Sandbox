//SimplePixelShader

struct PixelType
{
	float4 pos : SV_POSITION;
	float4 col : COLOR;
};

float4 ColorPixelShader(PixelType input) : SV_TARGET 
{
	return input.col;
}