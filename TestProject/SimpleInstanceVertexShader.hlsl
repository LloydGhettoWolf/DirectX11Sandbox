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
	PixelInput output;

	//multiply by world matrix
	float4 worldPos = mul(float4(vertIn.pos, 1.0f), world);

	//create local coordinate axes
	float4 eyeVec = float4(normalize(eyePos - (worldPos.xyz + vertIn.offset.xyz)), 0.0f);
	float4 rightVec = float4(normalize(cross(eyeVec.xyz, float3(0.0f, 1.0f, 0.0f))), 0.0f);
	float4 upVec = float4(normalize(cross(rightVec.xyz, eyeVec.xyz)), 0.0f);

	float4x4 rotationMatrix = float4x4(-rightVec,
										upVec,
										eyeVec,
										float4(0.0f, 0.0f, 0.0f, 1.0f));

	worldPos = mul(worldPos, rotationMatrix);
	output.pos = float4(worldPos.xyz + vertIn.offset.xyz, 1.0f);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, proj);
	output.lightCol = vertIn.color;
	output.uv = vertIn.uv;

	return output;
}