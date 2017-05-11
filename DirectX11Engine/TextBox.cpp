#include <D3D11.h>
#include <DirectXMath.h>
#include "TextBox.h"
#include <string.h>
#include "ResourceAllocator.h"
#include "Texture.h"
#include "FontInfo.h"

using namespace DirectX;

bool TextBox::Init(float xPos, float yPos, float scale, char* text, WCHAR* texturePath, Character* set, int fontHeight, int fontWidth,
	               ResourceAllocator* resourceAllocator, ID3D11Device* device)
{
	mXPos = xPos;
	mYPos = yPos;

	// Load Texture
	if (mTexture == nullptr)
	{
		mTexture = new Texture();
		if (!mTexture->InitFromDDS(device, texturePath))
			return false;
	}

	//get info about the string to present to screen
	int numChars = strlen(text);
	BufferInfo* info = new BufferInfo[numChars];

	for (int i = 0; i < numChars; i++)
	{
		int index = text[i] - INDEX_OFFSET;
		info[i].width = set[index].width;
		info[i].height = set[index].height;
		info[i].offsetX = set[index].x;
		info[i].offsetY = set[index].y;
	}

	mNumVerts = 4 * numChars;
	mNumIndices = 6 * numChars;

	if (!InitBuffers(info, scale, fontHeight, fontWidth, resourceAllocator))
		return false;
	
	delete[] info;

	return true;
}

bool TextBox::InitBuffers( BufferInfo* infos, float scale, int fontHeight, int fontWidth, ResourceAllocator* resourceAllocator)
{
	int individualVerts = mNumVerts * 3;
	float* vertices = new float[individualVerts];

	float startOffsetX = 0;

	int index = 0;
	
	for (int i = 0; i < individualVerts; i += 12)
	{
		float height = scale * infos[index].height;
		float width =  scale * infos[index].width;
		index++;

		vertices[i]     = mXPos + startOffsetX;
		vertices[i + 1] = mYPos - height;
		vertices[i + 2] = 0.0f;

		vertices[i + 3] = mXPos + startOffsetX;
		vertices[i + 4] = mYPos;
		vertices[i + 5] = 0.0f;

		vertices[i + 6] = mXPos + startOffsetX + width;
		vertices[i + 7] = mYPos;
		vertices[i + 8] = 0.0f;

		vertices[i + 9] = mXPos + startOffsetX + width;
		vertices[i + 10] = mYPos - height;
		vertices[i + 11] = 0.0f;



		startOffsetX += width;
	}

	int numUVs = mNumVerts * 2;
	float* uvs = new float[numUVs];

	index = 0;
	for (int i = 0; i < numUVs; i += 8)
	{

		float height = (float)infos[index].height/(float)fontHeight;
		float width  = (float)infos[index].width/(float)fontWidth;
		float startX = (float)infos[index].offsetX/(float)fontWidth;
		float startY = (float)infos[index].offsetY/(float)fontHeight;
		index++;

		uvs[i] = startX;
		uvs[i + 1] = startY + height;
		
		uvs[i + 2] = startX;
		uvs[i + 3] = startY;
		
		uvs[i + 4] = startX + width;
		uvs[i + 5] = startY;
	
		uvs[i + 6] = startX + width;
		uvs[i + 7] = startY + height;
		
		startOffsetX += width;
	}

	unsigned int* indices = new unsigned int[mNumIndices];

	int j = 0;
	for (int i = 0; i < mNumIndices; i += 6)
	{
		
		indices[i] = j;
		indices[i + 1] = j + 1;
		indices[i + 2] = j + 2;

		indices[i + 3] = j;
		indices[i + 4] = j + 2;
		indices[i + 5] = j + 3;

		j += 4;
	};

	if (mVertices != nullptr)
	{
		mVertices[0]->Release();
		mVertices[1]->Release();
	}
		
	mVertices = new ID3D11Buffer*[2];
	mVertices[0] = resourceAllocator->AllocateVertexBuffer(static_cast<void*>(vertices), sizeof(float) * 3, mNumVerts);
	if (mVertices[0] == nullptr)
		return false;

	mVertices[1] = resourceAllocator->AllocateVertexBuffer(static_cast<void*>(uvs), sizeof(float) * 2, mNumVerts);
	if (mVertices[1] == nullptr)
		return false;

	if (mIndices != nullptr)
		mIndices->Release();

	mIndices = resourceAllocator->AllocateIndexBuffer(static_cast<void*>(indices), sizeof(unsigned int), mNumIndices);
	if (mIndices == nullptr)
		return false;

	

	delete [] vertices;
	delete [] indices;
	delete [] uvs;

	return true;
}

ID3D11ShaderResourceView* TextBox::GetTexture()
{
	return mTexture->GetTexture();
}

void TextBox::Shutdown()
{
	if (mVertices)
	{
		mVertices[0]->Release();
		mVertices[1]->Release();
	}

	if (mIndices)
		mIndices->Release();

	if (mTexture)
		delete mTexture;
}
