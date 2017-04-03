struct PixelType
{
	float4 pos : SV_POSITION;
	float4 worldPos : POSITION;
	float3 norm : NORMAL;
	float2 tex : TEXCOORD0;
	float3 tangent : TANGENT;
};

cbuffer LightPositions
{
	float3 eyePos;
	float3 lightPos;
	float4 lightCol;
};

cbuffer MaterialProperties
{
	float4 diffuseCol;
	float4 specCol;
	float specComponent;
};

float4 SampleNormalMap(float3 normal, float3 tangent, float2 uv, Texture2D normalMap, SamplerState sampleType)
{
	float3 norm = normalize(normal);
	float3 tan = normalize(tangent);
	float4 normalSample = normalMap.Sample(sampleType, uv);

	//expand normal
	normalSample = (2.0f * normalSample) - 1.0f;

	float3 biTan = normalize(cross(normal, tangent));

	float4x4 tangentSpace = float4x4(float4(tangent, 0.0f),
		float4(biTan, 0.0f),
		float4(normal, 0.0f),
		float4(0.0f, 0.0f, 0.0f, 1.0f));

	return mul(normalSample, tangentSpace);
}

float4 DiffFactor(float3 lightVec, float3 newNorm, float2 uv, Texture2D diffuseMap, SamplerState sampleType)
{
	float diffCoeff = saturate(dot(lightVec, newNorm));
	float4 diffSample = diffuseMap.Sample(sampleType, uv);
	float4 diffFactor = diffCoeff * diffuseCol * lightCol * diffSample;
	return diffFactor;
}

float4 SpecFactor(float3 halfVec, float3 newNorm, float specComponent)
{
	float intensity = saturate(dot(halfVec, newNorm));
	float specCoeff = pow(intensity, specComponent);
	return  specCoeff * lightCol;
}

float4 SpecFactorFromSample(float3 halfVec, float3 newNorm, float2 uv, Texture2D specTexture, SamplerState sampleType)
{
	float intensity = saturate(dot(halfVec, newNorm));
	float specCoeff = pow(intensity, specComponent);
	float4 specSample = specTexture.Sample(sampleType, uv);
	return specCoeff * specSample * lightCol;
}
