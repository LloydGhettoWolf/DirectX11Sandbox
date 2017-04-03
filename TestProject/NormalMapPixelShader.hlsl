//SimplePixelShader

#include "NormalPixelShaderHeader.hlsl"

Texture2D diffTexture : register(t0);
Texture2D specTexture : register(t1);
Texture2D normTexture : register(t2);

SamplerState SampleType;


float4 main(PixelType input) : SV_TARGET0
{
	float3 norm = normalize(input.norm);
	float3 tan = normalize(input.tangent);

	float3 lightVec = normalize(lightPos - input.worldPos);
	float distance = length(lightPos - input.worldPos);
	float3 eyeVec = normalize(eyePos - input.worldPos);
	float3 halfVec = normalize(lightVec + eyeVec);

	float4 newNorm = SampleNormalMap(norm, tan, input.tex, normTexture, SampleType);
	float4 diffFactor = DiffFactor(lightVec, newNorm.xyz, input.tex, diffTexture, SampleType);
	float4 specFactor = SpecFactorFromSample(halfVec, newNorm, input.tex, specTexture, SampleType);

	float lightIntensity = 1.0f - (distance / 4000.0f);

	float4 amb = float4(0.05f, 0.05f, 0.05f, 1.0f);

	return saturate(lightIntensity * (amb + specFactor + diffFactor));
}