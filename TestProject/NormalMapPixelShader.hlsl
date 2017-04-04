//SimplePixelShader

#define NORMAL_MAP_PIXEL

#include "PixelShaderDefines.hlsl"

Texture2D diffTexture : register(t0);
Texture2D specTexture : register(t1);
Texture2D normTexture : register(t2);

SamplerState SampleType;


float4 main(NormalMapPixelType input) : SV_TARGET0
{
	
	float4 newNorm = SampleNormalMap(input.norm, input.tangent, input.tex, normTexture, SampleType);

	float3 lightVec = normalize(lightPos - input.worldPos);
	float distance = length(lightPos - input.worldPos);
	float3 eyeVec = normalize(eyePos - input.worldPos);

	float4 diffFactor = DiffFactor(lightVec, newNorm.xyz, input.tex, diffTexture, SampleType);
	float4 specFactor = SpecFactorFromSample(eyeVec, lightVec, newNorm, input.tex, specTexture, SampleType);
	float4 amb = float4(0.05f, 0.05f, 0.05f, 1.0f);

	float lightIntensity = AttenuateLight(distance);

	return saturate(lightIntensity * (amb + specFactor + diffFactor));
}