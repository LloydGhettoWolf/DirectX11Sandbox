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

cbuffer Positions
{
	float3 instPositions[1];
};

struct PixelInput
{
	float4 pos  : SV_POSITION;
	float2 uv   : TEXCOORD0;
};

struct VertInput
{
	float3 pos : POSITION;
	float2 uv : TEXCOORD0;
	float3 offset : INSTANCEPOS;
};

PixelInput main(VertInput vertIn)
{

	PixelInput output;

	//create local coordinate axes
	float4 eyeVec = float4(normalize(eyePos - (vertIn.pos + vertIn.offset)), 0.0f);
	float4 rightVec = float4(normalize(cross(eyeVec, float3(0.0f, 1.0f, 0.0f))), 0.0f);
	float4 upVec = float4(normalize(cross(rightVec, eyeVec)), 0.0f);

	float4x4 rotationMatrix = float4x4(-rightVec,
										upVec,
										eyeVec,
										float4(0.0f, 0.0f, 0.0f, 1.0f));

	output.pos = mul(float4(vertIn.pos, 1.0f), rotationMatrix);
	output.pos = float4(output.pos.xyz + vertIn.offset, 1.0f);
	output.pos = mul(output.pos, world);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, proj);

	output.uv = vertIn.uv;

	return output;
}