struct VSOutput
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};


VSOutput main(uint id : SV_VERTEXID)
{
	VSOutput output;

	output.pos.x = (float)(id / 2) * 4.0f - 1.0f;
	output.pos.y = (float)(id % 2) * 4.0f - 1.0f;
	output.pos.z = 0.0f;
	output.pos.w = 1.0f;

	output.uv.x = (float)(id / 2) * 2.0f;
	output.uv.y = 1.0 - (float)(id % 2) * 2.0f;

	return output;
}