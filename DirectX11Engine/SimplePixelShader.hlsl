//SimplePixelShader

struct PixelType
{
	float4 pos : SV_POSITION;
	float3 worldPos : POSITION;
	float3 norm : NORMAL;
	float2 tex : TEXCOORD;
};

Texture2D shaderTexture;
SamplerState SampleType;

cbuffer LightPositions
{
	float3 lightPos;
	float3 lightCol;
};

float4 main(PixelType input) : SV_TARGET 
{
	float3 norm = normalize(input.norm);
	
	float3 lightVec = normalize(lightPos - input.worldPos);
	float3 halfVec = normalize(norm + lightVec);

	float diffCoeff = saturate(dot(lightVec, norm));
	float specCoeff = pow(saturate(dot(halfVec, norm)), 16.0f);

	float3 diffCol = 0.1f * diffCoeff * lightCol;
	float3 specCol = specCoeff * lightCol;
	
	return saturate(float4(specCol + diffCol, 1.0f));
}