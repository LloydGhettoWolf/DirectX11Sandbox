//SimplePixelShader

#define STANDARD_PIXEL
#include "PixelShaderDefines.hlsl"

Texture2D diffTexture : register(t0);
Texture2D specTexture : register(t1);
SamplerState SampleType;


float4 main(StandardPixelType input) : SV_TARGET0
{
	float3 norm = normalize(input.norm);
	float3 lightVec = normalize(lightPos - input.worldPos.xyz);
	float distance = length(lightPos - input.worldPos.xyz);
	float3 eyeVec = normalize(eyePos - input.worldPos.xyz);

	float4 diffSample = diffTexture.Sample(SampleType, input.tex);
	float4 specSample = specTexture.Sample(SampleType, input.tex);

	float specFactor = SpecFactor(eyeVec, lightVec, norm, specComponent);
	
	float4 diffFactor = DiffFactor(lightVec, input.norm, diffSample);
	float4 amb = float4(0.1f, 0.1f, 0.1f, 1.0f) * diffSample;

	float lightIntensity = AttenuateLight(distance);
	return saturate(amb + lightIntensity * lightColor * (specFactor * specSample + diffFactor * diffColor));
}