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

struct EyeBufferType
{
	XMFLOAT3 eyePos;
	float padding;
};

struct LightPosBuffer
{
	XMFLOAT4 lightPos;
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
	EyeBufferType* eyePtr;
};

struct InstancedConstantsStruct
{
	MatrixBufferType* matPtr;
	EyeBufferType* eyePtr;
};

struct ShaderPerMeshStruct
{
	MaterialProperties* material;
	ID3D11SamplerState* sampler;
	ID3D11ShaderResourceView* diffuseSrv;
	ID3D11ShaderResourceView* specSrv;
	ID3D11ShaderResourceView* normSrv;
	ID3D11ShaderResourceView* maskSrv;
};