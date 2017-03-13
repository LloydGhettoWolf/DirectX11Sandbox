//SimplePixelShader

struct PixelType
{
	float4 pos : SV_POSITION;
	float4 worldPos : POSITION;
	float4 norm : NORMAL;
	float4 tex : TEXCOORD0;
};

Texture2D shaderTexture;
SamplerState SampleType;

cbuffer LightPositions
{
	float3 lightPos;
	float4 lightCol;
};

cbuffer MaterialProperties
{
	float4 diffuseCol;
	float4 specCol;
	float specComponent;
};

float4 main(PixelType input) : SV_TARGET 
{
	float3 norm = normalize(input.norm);
	
	float3 lightVec = normalize(lightPos - input.worldPos);
	float3 halfVec = normalize(norm + lightVec);

	float diffCoeff = saturate(dot(lightVec, norm));
	float specCoeff = pow(saturate(dot(halfVec, norm)), specComponent);

	float4 texSample = shaderTexture.Sample(SampleType, -input.tex.xy);

	float4 diffFactor = diffCoeff * diffuseCol * lightCol;
	float4 specFactor = specCoeff * specCol * lightCol;
	
	return  saturate(texSample);
}