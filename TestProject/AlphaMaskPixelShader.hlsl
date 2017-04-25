#define STANDARD_PIXEL
#include "PixelShaderDefines.hlsl"

Texture2D diffTexture : register(t0);
Texture2D alphaTexture : register(t1);
SamplerState SampleType;

float4 main(StandardPixelType input) : SV_TARGET
{
	float3 norm = normalize(input.norm);
	float3 lightVec = normalize(lights[0].lightPos - input.worldPos).xyz;
	float distance = length(lights[0].lightPos - input.worldPos);
	float4 diffSample = diffTexture.Sample(SampleType, input.tex);

	float4 diffFactor = DiffFactor(lightVec, norm.xyz, diffSample);
	float4 amb = float4(0.1f, 0.1f, 0.1f, 1.0f) * diffSample;
	float4 alpha = alphaTexture.Sample(SampleType, input.tex);
	float lightIntensity = AttenuateLight(distance);
	return float4(amb.xyz + lightIntensity * (diffFactor.xyz), alpha.r);
}