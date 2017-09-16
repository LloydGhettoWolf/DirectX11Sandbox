//SimplePixelShader
#define NORMAL_MAP_PIXEL

#include "PixelShaderDefines.hlsl"

Texture2D diffTexture : register(t0);
Texture2D normTexture : register(t1);

SamplerState SampleType;

[earlydepthstencil]
float4 main(NormalMapPixelType input) : SV_TARGET0
{
	float3 eyeVec = normalize(eyePos - input.worldPos.xyz);

	float4 diffSample = diffTexture.Sample(SampleType, input.tex);
	float4 normSample = normTexture.Sample(SampleType, input.tex);

	float4 norm = SampleNormalMap(input.norm, input.tangent, normSample);

	float4 accum = float4(0.0f, 0.0f, 0.0f, 1.0f);

	[loop]
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		float3 lightVec = lights[i].lightPos.xyz - input.worldPos.xyz;
		float  distance = length(lightVec);
		lightVec = normalize(lightVec);
		float4 diffFactor = DiffFactor(lightVec, norm.xyz, diffSample);
		float lightIntensity = AttenuateLight(distance);

		accum += lights[i].lightColor * lightIntensity *  (diffFactor * diffColor);
	}

	float4 amb = float4(0.05f, 0.05f, 0.05f, 1.0f) * diffSample;

	return saturate(accum);
}