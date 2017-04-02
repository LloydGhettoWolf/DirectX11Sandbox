//SimplePixelShader

struct PixelType
{
	float4 pos : SV_POSITION;
	float4 worldPos : POSITION;
	float3 norm : NORMAL;
	float2 tex : TEXCOORD0;
	float3 tangent : TANGENT;
};

Texture2D diffTexture : register(t0);
Texture2D normTexture : register(t1);

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
	float3 tan = normalize(input.tangent);
	float3 biTan = normalize(cross(norm,tan));

	float4x4 tangentSpace = float4x4(float4(norm, 1.0f),
									 float4(tan, 1.0f),
									 float4(biTan, 1.0f),
									 float4(0.0f, 0.0f, 0.0f, 1.0f));

	float3 lightVec = normalize(lightPos - input.worldPos);
	float distance = length(lightPos - input.worldPos);
	float3 eyeVec = normalize(eyePos - input.worldPos);
	float3 halfVec = normalize(lightVec + eyeVec);

	float4 normalSample = normTexture.Sample(SampleType, input.tex);
	normalSample = mul(normalSample, tangentSpace);

	float intensity = saturate(dot(halfVec, normalSample.xyz));
	float specCoeff = pow(intensity, specComponent);

	float4 specFactor = specCoeff * lightCol;

	float diffCoeff = saturate(dot(lightVec, normalSample.xyz));
	float4 diffSample = diffTexture.Sample(SampleType, input.tex);
	float4 diffFactor = diffCoeff * diffuseCol * lightCol * diffSample;

	float lightIntensity = 1.0f - (distance / 4000.0f);

	float4 amb = float4(0.05f, 0.05f, 0.05f, 1.0f);

	return saturate(lightIntensity * (amb + specFactor + diffFactor));
}