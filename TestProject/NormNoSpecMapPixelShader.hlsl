//SimplePixelShader
#define NORMAL_MAP_PIXEL

#include "PixelShaderDefines.hlsl"

Texture2D diffTexture : register(t0);
Texture2D normTexture : register(t1);

SamplerState SampleType;


float4 main(NormalMapPixelType input) : SV_TARGET0
{
	float3 lightVec = normalize(lightPos - input.worldPos);
	float distance = length(lightPos - input.worldPos);
	float3 eyeVec = normalize(eyePos - input.worldPos);

	float4 diffSample = diffTexture.Sample(SampleType, input.tex);
	float4 normSample = normTexture.Sample(SampleType, input.tex);

	float4 norm = SampleNormalMap(input.norm, input.tangent, normSample);

	float4 diffFactor = DiffFactor(lightVec, norm.xyz, diffSample);
	float4 amb = float4(0.1f, 0.1f, 0.1f, 1.0f) * diffSample;
	float lightIntensity = AttenuateLight(distance);

	return saturate(amb + lightIntensity * (lightColor * diffFactor * diffColor));
}