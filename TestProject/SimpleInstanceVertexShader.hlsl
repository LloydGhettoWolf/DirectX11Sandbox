//Simple Vert shader
cbuffer Matrices : register(b0)
{
	matrix world;
	matrix view;
	matrix proj;
}

cbuffer EyeInfo : register(b1)
{
	float3 eyePos;
};


struct PixelInput
{
	float4 pos  : SV_POSITION;
	float2 uv   : TEXCOORD0;
	float4 lightCol : POSITION;
};

struct VertInput
{
	float3 pos : POSITION;
	float2 uv : TEXCOORD0;
	float4 offset : INSTANCEPOS;
	float4 color  : INSTANCECOL;
};

PixelInput main(VertInput vertIn)
{
	float4 worldPos = float4(vertIn.pos + vertIn.offset, 1.0f);
	//create local coordinate axes
	float4 eyeVec = float4(normalize(eyePos - worldPos), 0.0f);
	float4 rightVec = float4(normalize(cross(eyeVec.xyz, float3(0.0f, 1.0f, 0.0f))), 0.0f);
	float4 upVec = float4(normalize(cross(rightVec.xyz, eyeVec.xyz)), 0.0f);

	//create rotation matrix to angle the billboard toward the camera
	float4x4 rotationMatrix = float4x4(-rightVec,
										upVec,
										eyeVec,
										float4(0.0f, 0.0f, 0.0f, 1.0f));

	PixelInput output;
	worldPos = mul(vertIn.pos, rotationMatrix);
	worldPos += vertIn.offset;
	worldPos = mul(worldPos, view);
	output.pos = mul(worldPos, proj);
	output.lightCol = vertIn.color;
	output.uv = vertIn.uv;

	return output;
}