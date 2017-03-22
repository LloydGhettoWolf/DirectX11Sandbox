struct PixelType
{
	float4 pos : SV_POSITION;
};

float4 main(PixelType input) : SV_TARGET
{
	return float4(0.0f, 1.0f, 0.0f, 1.0f);
}