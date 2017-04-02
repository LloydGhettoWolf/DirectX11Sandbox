//simpleMesh.cpp
#include <d3d11.h>
#include "SimpleMesh.h"
#include "Texture.h"
#include "ObjParser.h"


bool SimpleMesh::Init(ID3D11Device* device,ProcessedMeshData* mesh)
{
	if (!InitBuffers(device, mesh))
		return false;

	if (!InitBoundingBox(device))
		return false;

	return true;
}


void SimpleMesh::Shutdown() {
	ShutdownBuffers();
}


bool SimpleMesh::InitBuffers(ID3D11Device* device, ProcessedMeshData* mesh)
{
	D3D11_SUBRESOURCE_DATA vertData, normalData, uvData, tangentData, indexData;
	HRESULT result;

	D3D11_BUFFER_DESC* vertBufferDesc = new D3D11_BUFFER_DESC;
	
	D3D11_BUFFER_DESC* indexBufferDesc = new D3D11_BUFFER_DESC;

	mVertCount = mesh->numVerts;
	mIndexCount = mesh->numIndices;

	
	mVertCount = mesh->numVerts;
	mIndexCount = mesh->numIndices;

	vertBufferDesc->Usage = D3D11_USAGE_DEFAULT;
	vertBufferDesc->BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertBufferDesc->CPUAccessFlags = 0;
	vertBufferDesc->MiscFlags = 0;
	vertBufferDesc->StructureByteStride = 0;
	vertBufferDesc->ByteWidth = sizeof(XMFLOAT3) * mVertCount;

	vertData.pSysMem = &mesh->vertices[0];
	vertData.SysMemPitch = 0;
	vertData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(vertBufferDesc, &vertData, &mVertBuffer);
	if (FAILED(result))
	{
		return false;
	}

	
	vertBufferDesc->ByteWidth = sizeof(XMFLOAT2) * mVertCount;
	uvData.pSysMem = &mesh->uvs[0];
	uvData.SysMemPitch = 0;
	uvData.SysMemSlicePitch = 0;

	// Now create the uv buffer.
	result = device->CreateBuffer(vertBufferDesc, &uvData, &mUvBuffer);
	if (FAILED(result))
	{
		return false;
	}

	vertBufferDesc->ByteWidth = sizeof(XMFLOAT3) * mVertCount;
	normalData.pSysMem = &mesh->normals[0];
	normalData.SysMemPitch = 0;
	normalData.SysMemSlicePitch = 0;

	// Now create the normal buffer.
	result = device->CreateBuffer(vertBufferDesc, &normalData, &mNormalBuffer);
	if (FAILED(result))
	{
		return false;
	}

	
	if (mesh->isNormalMapped)
	{
		vertBufferDesc->ByteWidth = sizeof(XMFLOAT3) * mVertCount;
		tangentData.pSysMem = &mesh->tangents[0];
		tangentData.SysMemPitch = 0;
		tangentData.SysMemSlicePitch = 0;
		mIsNormalMapped = true;

		result = device->CreateBuffer(vertBufferDesc, &tangentData, &mTangentBuffer);
		if (FAILED(result))
		{
			return false;
		}

	}

	// Set up the description of the static index buffer.
	indexBufferDesc->Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc->ByteWidth = sizeof(unsigned int) * mIndexCount;
	indexBufferDesc->BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc->CPUAccessFlags = 0;
	indexBufferDesc->MiscFlags = 0;
	indexBufferDesc->StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = &mesh->indices[0];
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(indexBufferDesc, &indexData, &mIndexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	mMaterialIndex = mesh->materialIndex;

	min = mesh->min;
	max = mesh->max;

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


void SimpleMesh::Render(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride3, stride2;
	unsigned int offset;

	// Set vertex buffer stride and offset.

	stride3 = sizeof(XMFLOAT3);
	stride2 = sizeof(XMFLOAT2);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &mVertBuffer, &stride3, &offset);
	deviceContext->IASetVertexBuffers(1, 1, &mNormalBuffer, &stride3, &offset);
	deviceContext->IASetVertexBuffers(2, 1, &mUvBuffer, &stride2, &offset);

	if (mIsNormalMapped)
	{
		deviceContext->IASetVertexBuffers(3, 1, &mTangentBuffer, &stride3, &offset);
	}

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//now we're set up, let's render!
	deviceContext->DrawIndexed(mIndexCount, 0, 0);
	
	return;
}

void SimpleMesh::RenderBoundingBox(ID3D11DeviceContext* deviceContext)
{
	unsigned int offset;
	unsigned int stride;
	offset = 0;

	// Set vertex buffer stride and offset.
	stride = sizeof(XMFLOAT4);

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &mAABBVertBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(mAABBIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	//now we're set up, let's render!
	deviceContext->DrawIndexed(24, 0, 0);
}

void SimpleMesh::ShutdownBuffers()
{
	// Release the index buffer.
	if (mIndexBuffer)
	{
		mIndexBuffer->Release();
		//delete mIndexBuffer;
	}

	// Release the vertex buffer.
	if (mVertBuffer)
	{
		mVertBuffer->Release();
		mNormalBuffer->Release();
		mUvBuffer->Release();
		if (mTangentBuffer != nullptr)
		{
			mTangentBuffer->Release();
		}
		//delete mVertBuffer;
	}

	return;
}