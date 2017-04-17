//Simple Vert shader
cbuffer Matrices
{
	matrix world;
	matrix view;
	matrix proj;
}

cbuffer EyeInfo
{
	float3 eyePos;
};


struct PixelInput
{
	float4 pos  : SV_POSITION;
	float2 uv   : TEXCOORD0;
	float3 lightCol : POSITION;
};

struct VertInput
{
	float3 pos : POSITION;
	float2 uv : TEXCOORD0;
	float3 offset : INSTANCEPOS;
	float3 color  : INSTANCECOL;
};

PixelInput main(VertInput vertIn)
{
	PixelInput output;

	//multiply by world matrix
	float4 worldPos = mul(vertIn.pos, world);

	//create local coordinate axes
	float4 eyeVec = float4(normalize(eyePos - (worldPos + vertIn.offset)), 0.0f);
	float4 rightVec = float4(normalize(cross(eyeVec, float3(0.0f, 1.0f, 0.0f))), 0.0f);
	float4 upVec = float4(normalize(cross(rightVec, eyeVec)), 0.0f);

	float4x4 rotationMatrix = float4x4(-rightVec,
										upVec,
										eyeVec,
										float4(0.0f, 0.0f, 0.0f, 1.0f));

	worldPos = mul(worldPos, rotationMatrix);
	output.pos = float4(worldPos.xyz + vertIn.offset, 1.0f);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, proj);
	output.lightCol = vertIn.color;
	output.uv = vertIn.uv;

	return output;
}