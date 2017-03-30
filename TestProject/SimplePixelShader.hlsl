//SimplePixelShader

struct PixelType
{
	float4 pos : SV_POSITION;
	float4 worldPos : POSITION;
	float3 norm : NORMAL;
	float2 tex : TEXCOORD0;
};

Texture2D shaderTexture;
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
	float diffCoeff = saturate(dot(lightVec, norm));
	float4 texSample = shaderTexture.Sample(SampleType, input.tex);
	float4 diffFactor = diffCoeff * diffuseCol * lightCol;
	float4 amb = float4(0.2f, 0.2f, 0.2f, 1.0f);
	float distance = length(lightPos - input.worldPos);
	float lightIntensity = 1.0f - (distance / 4000.0f);
	return  saturate((diffFactor + amb) * lightIntensity * texSample);
}