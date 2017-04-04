//SimplePixelShader

#define STANDARD_PIXEL

#include "PixelShaderDefines.hlsl"

Texture2D diffTexture : register(t0);

SamplerState SampleType;

float4 main(StandardPixelType input) : SV_TARGET0
{
	float3 norm = normalize(input.norm);
	float3 lightVec = normalize(lightPos - input.worldPos);

	float4 diffFactor = DiffFactor(lightVec, input.norm, input.tex, diffTexture, SampleType);
	float4 amb = float4(0.2f, 0.2f, 0.2f, 1.0f);

	float distance = length(lightPos - input.worldPos);
	float lightIntensity = AttenuateLight(distance);

	return  saturate((diffFactor + amb) * lightIntensity);
}