
struct PixelInput
{
	float4 pos  : SV_POSITION;
	float2 uv : TEXCOORD0;
};

Texture2D pointTexture : register(t0);
SamplerState SampleType;

float4 main(PixelInput input) : SV_TARGET
{
	float4 colorSample = pointTexture.Sample(SampleType, input.uv);
	colorSample.a = colorSample.r;
	return colorSample;
}