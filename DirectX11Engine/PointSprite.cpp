#include <d3d11.h>
#include "Texture.h"
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

bool PointSprite::Init(ResourceAllocator* resourceAllocator, ID3D11Device* device, WCHAR* fileName)
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

	mInstanceBuffer = resourceAllocator->AllocateVInstanceBuffer(static_cast<void*>(&mPositions[0]), sizeof(XMFLOAT3) * 2, mNumSprites);
	if (mInstanceBuffer == nullptr)
	{
		return false;
	}


	mPointTexture = new Texture();
	bool result = mPointTexture->InitFromDDS(device, &fileName[0]);
	if (!result)
		return false;

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

ID3D11Buffer** PointSprite::GetAllBuffers()
{
	ID3D11Buffer* vertBuffers[3] = { mVertexBuffer, mUVBuffer, mInstanceBuffer };
	return &vertBuffers[0];
}

ID3D11ShaderResourceView* PointSprite::GetTexture()
{
	return mPointTexture->GetTexture();
}
	
bool PointSprite::Update(ID3D11DeviceContext* context, XMFLOAT3* positions)
{
	//update instanced data
	D3D11_MAPPED_SUBRESOURCE newPositionData;

	// Lock the constant buffer so it can be written to.
	HRESULT result = context->Map(mInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &newPositionData);
	if (FAILED(result))
	{
		return false;
	}

	XMFLOAT3* dataPtr;
	dataPtr = (XMFLOAT3*)newPositionData.pData;
	dataPtr[0] = positions[0];
	dataPtr[1] = positions[1];
	context->Unmap(mInstanceBuffer, 0);
};


