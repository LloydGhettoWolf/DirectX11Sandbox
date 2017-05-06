//SimplePixelShader

#define STANDARD_PIXEL

#include "PixelShaderDefines.hlsl"

Texture2D diffTexture : register(t0);

SamplerState SampleType;

[earlydepthstencil]
float4 main(StandardPixelType input) : SV_TARGET0
{
	float3 norm = normalize(input.norm);
	float3 eyeVec = normalize(eyePos - input.worldPos.xyz);
	float4 diffSample = diffTexture.Sample(SampleType, input.tex);
	float4 accum = float4(0.0f, 0.0f, 0.0f, 1.0f);
	
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		float3 lightVec = normalize(lights[i].lightPos.xyz - input.worldPos.xyz);
		float distance = length(lights[i].lightPos.xyz - input.worldPos.xyz);
		float specFactor = saturate(SpecFactor(eyeVec, lightVec, norm, specComponent));
		float4 diffFactor = saturate(DiffFactor(lightVec, norm, diffSample));
		float lightIntensity = AttenuateLight(distance);

		accum += lights[i].lightColor * lightIntensity *  (specFactor + diffFactor * diffColor);
	}

	float4 amb = float4(0.05f, 0.05f, 0.05f, 1.0f) * diffSample;

	return  saturate(accum);
}