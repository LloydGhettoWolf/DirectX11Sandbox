//SimplePixelShader

struct PixelType
{
	float4 pos : SV_POSITION;
	float4 worldPos : POSITION;
	float3 norm : NORMAL;
	float2 tex : TEXCOORD0;
};

Texture2D diffTexture : register(t0);
Texture2D specTexture : register(t1);
SamplerState SampleType;

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

float4 main(PixelType input) : SV_TARGET0
{
	float3 norm = normalize(input.norm);
	float3 lightVec = normalize(lightPos - input.worldPos);
	float distance = length(lightPos - input.worldPos);
	float3 eyeVec = normalize(eyePos - input.worldPos);
	float3 halfVec = normalize(lightVec + eyeVec);

	float intensity = saturate(dot(halfVec, norm));
	float specCoeff = pow(intensity, specComponent);

	float specColor = specTexture.Sample(SampleType, input.tex);
	float4 specFactor = specCoeff * specColor * lightCol;

	float diffCoeff = saturate(dot(lightVec, norm));
	float4 diffTexColor = diffTexture.Sample(SampleType, input.tex);
	float4 diffFactor = diffCoeff * diffuseCol * lightCol * diffTexColor;

	float lightIntensity = 1.0f - (distance / 4000.0f);

	float4 amb = float4(0.05f, 0.05f, 0.05f, 1.0f);

	return  saturate(lightIntensity * (amb + specFactor + diffFactor));
}