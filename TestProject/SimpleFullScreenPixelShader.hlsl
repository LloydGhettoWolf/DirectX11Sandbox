struct PSInput
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

Texture2D tex: register(t0);
SamplerState samplerType;

float4 main(PSInput input) : SV_TARGET
{
	float4 depth = tex.Sample(samplerType, input.uv);
	return depth;
}