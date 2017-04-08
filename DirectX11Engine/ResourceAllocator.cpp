#include <d3d11.h>
#include "ResourceAllocator.h"

ID3D11Buffer* ResourceAllocator::AllocateVertexBuffer(void* data, unsigned int elemSize, unsigned int count)
{
	D3D11_BUFFER_DESC vertBufferDesc; 
	D3D11_SUBRESOURCE_DATA vertData;
	ID3D11Buffer* vertBuffer;

	vertBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertBufferDesc.CPUAccessFlags = 0;
	vertBufferDesc.MiscFlags = 0;
	vertBufferDesc.StructureByteStride = 0;
	vertBufferDesc.ByteWidth = elemSize * count;

	vertData.pSysMem = data;
	vertData.SysMemPitch = 0;
	vertData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	HRESULT result = mDevice->CreateBuffer(&vertBufferDesc, &vertData, &vertBuffer);
	if (FAILED(result))
	{
		return nullptr;
	}

	return vertBuffer;
}

ID3D11Buffer* ResourceAllocator::AllocateIndexBuffer(void* data, unsigned int elemSize, unsigned int count)
{
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA indexData;
	ID3D11Buffer* indexBuffer;

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = elemSize * count;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = data;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	HRESULT result = mDevice->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
	if (FAILED(result))
	{
		return nullptr;
	}

	return indexBuffer;
}