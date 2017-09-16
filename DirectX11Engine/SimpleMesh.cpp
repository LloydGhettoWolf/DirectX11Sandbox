//simpleMesh.cpp
#include <d3d11.h>
#include "SimpleMesh.h"
#include "ObjParser.h"
#include "ResourceAllocator.h"

bool SimpleMesh::Init(ResourceAllocator* resourceAllocator, ProcessedMeshData* mesh)
{
	if (!InitBuffers(resourceAllocator, mesh))
		return false;

#ifdef RENDER_AABB
	if (!InitBoundingBox(device))
		return false;
#endif

	return true;
}

void SimpleMesh::Shutdown() 
{
	// Release the index buffer.
	if (mIndexBuffer)
	{
		mIndexBuffer->Release();
	}

	// Release the vertex buffer.
	if (mVertBuffer)
	{
		for (unsigned int i = 0; i < mNumBuffers; i++)
		{
			mVertBuffer[i]->Release();
			delete mVertBuffer[i];
		}
	}

#ifdef RENDER_AABB
	if (mAABBVertBuffer)
	{
		mAABBVertBuffer->Release();
	}

	if (mAABBIndexBuffer)
	{
		mAABBIndexBuffer->Release();
	}
#endif
}

bool SimpleMesh::InitBuffers(ResourceAllocator* resourceAllocator, ProcessedMeshData* mesh)
{
	mVertCount = mesh->numVerts;
	mIndexCount = mesh->numIndices;

	if (!mesh->isNormalMapped)
	{
		
		if (!mesh->normals)
		{
			mNumBuffers = 2;
			mVertBuffer = new ID3D11Buffer*[2];

			mStrides[0] = sizeof(XMFLOAT3);
			mStrides[1] = sizeof(XMFLOAT2);
		}
		else
		{
			mNumBuffers = 3;
			mVertBuffer = new ID3D11Buffer*[3];

			mStrides[0] = sizeof(XMFLOAT3);
			mStrides[1] = sizeof(XMFLOAT3);
			mStrides[2] = sizeof(XMFLOAT2);
		}
	}
	else
	{
		mNumBuffers = 4;
		mVertBuffer = new ID3D11Buffer*[4];

		mStrides[0] = sizeof(XMFLOAT3);
		mStrides[1] = sizeof(XMFLOAT3);
		mStrides[2] = sizeof(XMFLOAT2);
		mStrides[3] = sizeof(XMFLOAT3);
	}


	mVertBuffer[0] = resourceAllocator->AllocateVertexBuffer(static_cast<void*>(&mesh->vertices[0]), mStrides[0], mVertCount);
	if (mVertBuffer[0] == nullptr)
	{
		return false;
	}

	if (!mesh->normals)
	{
		mVertBuffer[1] = resourceAllocator->AllocateVertexBuffer(static_cast<void*>(&mesh->uvs[0]), mStrides[1], mVertCount);
		if (mVertBuffer[1] == nullptr)
		{
			return false;
		}
	}
	else
	{
		mVertBuffer[1] = resourceAllocator->AllocateVertexBuffer(static_cast<void*>(&mesh->normals[0]), mStrides[1], mVertCount);
		if (mVertBuffer[1] == nullptr)
		{
			return false;
		}

		mVertBuffer[2] = resourceAllocator->AllocateVertexBuffer(static_cast<void*>(&mesh->uvs[0]), mStrides[2], mVertCount);
		if (mVertBuffer[2] == nullptr)
		{
			return false;
		}
	}

	if (mesh->isNormalMapped)
	{
		mVertBuffer[3] = resourceAllocator->AllocateVertexBuffer(static_cast<void*>(&mesh->tangents[0]), mStrides[3], mVertCount);
		if (mVertBuffer[3] == nullptr)
		{
			return false;
		}
	}

	mIndexBuffer = resourceAllocator->AllocateIndexBuffer(static_cast<void*>(&mesh->indices[0]), sizeof(unsigned int), mIndexCount);
	if (mIndexBuffer == nullptr)
	{
		return false;
	}

	mMaterialIndex = mesh->materialIndex;
	min = mesh->min;
	max = mesh->max;
	center = mesh->center;

	return true;
}

bool SimpleMesh::InitBoundingBox(ID3D11Device* device)
{
	D3D11_SUBRESOURCE_DATA vertData, indexData;
	HRESULT result;

	D3D11_BUFFER_DESC* vertBufferDesc = new D3D11_BUFFER_DESC;
	D3D11_BUFFER_DESC* indexBufferDesc = new D3D11_BUFFER_DESC;


	vertBufferDesc->Usage = D3D11_USAGE_DEFAULT;
	vertBufferDesc->ByteWidth = sizeof(XMFLOAT4) * 8; //aabb has 8 points
	vertBufferDesc->BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertBufferDesc->CPUAccessFlags = 0;
	vertBufferDesc->MiscFlags = 0;
	vertBufferDesc->StructureByteStride = 0;

	XMFLOAT3 aabbVerts[8] =
	{
		//top
		XMFLOAT3(min.x, min.y, min.z),
		XMFLOAT3(max.x, min.y, min.z),
		XMFLOAT3(max.x, min.y, max.z),
		XMFLOAT3(min.x, min.y, max.z),
			   
		//bottom
		XMFLOAT3(min.x, max.y, min.z),
		XMFLOAT3(max.x, max.y, min.z),
		XMFLOAT3(max.x, max.y, max.z),
		XMFLOAT3(min.x, max.y, max.z),
	};


	// Give the subresource structure a pointer to the vertex data.
	vertData.pSysMem = &aabbVerts[0];
	vertData.SysMemPitch = 0;
	vertData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(vertBufferDesc, &vertData, &mAABBVertBuffer);
	if (FAILED(result))
	{
		return false;
	}

	unsigned int indices[24] =
	{
		//top
		0,1, 1,2, 2,3, 3,0,
		//bottom
		4,5, 5,6, 6,7, 7,4,

		//sides
		0,4, 1,5, 2,6, 3,7
	};

	// Set up the description of the static index buffer.
	indexBufferDesc->Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc->ByteWidth = sizeof(unsigned int) * 24;
	indexBufferDesc->BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc->CPUAccessFlags = 0;
	indexBufferDesc->MiscFlags = 0;
	indexBufferDesc->StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = &indices[0];
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(indexBufferDesc, &indexData, &mAABBIndexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}