#include <d3d11.h>
#include "PointSprite.h"
#include "ResourceAllocator.h"


using namespace DirectX;

XMFLOAT3 vertices[4] = 
{
	XMFLOAT3(-100.0f, 100.0f, 0.0f),
	XMFLOAT3(-100.0f, -100.0f, 0.0f),
	XMFLOAT3(100.0f, -100.0f, 0.0f),
	XMFLOAT3(100.0f, 100.0f, 0.0f)
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
