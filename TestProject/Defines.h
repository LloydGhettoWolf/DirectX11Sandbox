#pragma once
#include <DirectXMath.h>

using namespace DirectX;

#define RENDER_AABB 0

struct MatrixBufferType
{
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;
};

struct LightPosBuffer
{
	XMFLOAT3 eyePos;
	float padding;
	XMFLOAT3 lightPos;
	float padding1;
	XMFLOAT4 lightCol;
};

struct MaterialProperties
{
	XMFLOAT4 diffuseCol;
	XMFLOAT4 specCol;
	float specComponent;
	float padding[3];
};

struct ConstantsStruct
{
	MatrixBufferType* matPtr;
	LightPosBuffer* lightPtr;
};