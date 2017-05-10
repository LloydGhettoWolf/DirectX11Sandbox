#include <D3D11.h>
#include <DirectXMath.h>
#include "TextBox.h"
#include "ResourceAllocator.h"

using namespace DirectX;

bool TextBox::Init(float width, float height, float xPos, float yPos, ResourceAllocator* resourceAllocator)
{
	mRectHeight = height;
	mRectWidth = width;

	mXPos = xPos;
	mYPos = yPos;

	if (!InitBuffers(resourceAllocator))
		return false;
	
	return true;
}

bool TextBox::InitBuffers(ResourceAllocator* resourceAllocator)
{
	float vertices[12] =
	{
		mXPos, mYPos - mRectHeight, 0.0f,
		mXPos, mYPos, 0.0f,
		mXPos + mRectWidth, mYPos, 0.0f,
		mXPos + mRectWidth, mYPos - mRectHeight, 0.0f
	};

	unsigned int indices[6] =
	{
		0, 1, 2,
		0, 2, 3
	};

	mVertices = resourceAllocator->AllocateVertexBuffer(static_cast<void*>(vertices), sizeof(float) * 3, 4);
	if (mVertices == nullptr)
		return false;

	mIndices = resourceAllocator->AllocateIndexBuffer(static_cast<void*>(indices), sizeof(unsigned int), 6);
	if (mIndices == nullptr)
		return false;

	return true;
}

void TextBox::Shutdown()
{
	if (mVertices)
		mVertices->Release();

	if (mIndices)
		mIndices->Release();
}
