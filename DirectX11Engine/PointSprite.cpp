#include <d3d11.h>
#include "PointSprite.h"
#include "ResourceAllocator.h"


using namespace DirectX;

const float WIDTH = 800;
const float HEIGHT = 600;

const float ASPECT = WIDTH / HEIGHT;
const float RATIO = 1.0f / ASPECT;

XMFLOAT3 vertices[4] = 
{
	XMFLOAT3(RATIO * -25.0f, 25.0f, 0.0f),
	XMFLOAT3(RATIO * -25.0f, -25.0f, 0.0f),
	XMFLOAT3(RATIO * 25.0f, -25.0f, 0.0f),
	XMFLOAT3(RATIO * 25.0f, 25.0f, 0.0f)
};

XMFLOAT2 uvs[4] =
{
	XMFLOAT2(0.0f, 0.0f),
	XMFLOAT2(0.0f, 1.0f),
	XMFLOAT2(1.0f, 1.0f),
	XMFLOAT2(1.0f, 0.0f)
};

unsigned int indices[6] =
{
	0,1,2,
	0,2,3
};

bool PointSprite::Init(ResourceAllocator* resourceAllocator)
{
	mVertexBuffer = resourceAllocator->AllocateVertexBuffer(static_cast<void*>(&vertices[0]), sizeof(XMFLOAT3), 4);
	if (mVertexBuffer == nullptr)
	{
		return false;
	}

	mUVBuffer = resourceAllocator->AllocateVertexBuffer(static_cast<void*>(&uvs[0]), sizeof(XMFLOAT2), 4);
	if (mUVBuffer == nullptr)
	{
		return false;
	}

	mIndexBuffer = resourceAllocator->AllocateIndexBuffer(static_cast<void*>(&indices[0]), sizeof(unsigned int), 6);
	if (mIndexBuffer == nullptr)
	{
		return false;
	}

	mInstanceBuffer = resourceAllocator->AllocateVInstanceBuffer(static_cast<void*>(&mPositions[0]), sizeof(XMFLOAT3), mNumSprites);
	if (mInstanceBuffer == nullptr)
	{
		return false;
	}

	return true;
}

void PointSprite::Shutdown()
{
	if (mVertexBuffer)
	{
		mVertexBuffer->Release();
	}

	if (mIndexBuffer)
	{
		mIndexBuffer->Release();
	}

	if (mInstanceBuffer)
	{
		mInstanceBuffer->Release();
	}

}

	
void PointSprite::Update()
{
};
