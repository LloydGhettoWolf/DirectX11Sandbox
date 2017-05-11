struct PixelType
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

Texture2D pointTexture : register(t0);
SamplerState SampleType;

float4 main(PixelType input) : SV_TARGET
{
	float4 col = pointTexture.Sample(SampleType, input.uv);
	return col.x * col + (1.0f - col.x) * float4(1.0f, 0.0f, 0.0f, 0.55f);
}