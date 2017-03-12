//simpleMesh.cpp
#include "SimpleMesh.h"

bool SimpleMesh::Init(ID3D11Device* device,ProcessedMeshData* mesh)
{
	if (!InitBuffers(device, mesh))
		return false;

	//Init Textures
	//if (!InitTextures(device, mesh))
		//return false;

	return true;
}

void SimpleMesh::Shutdown() {
	ShutdownBuffers();
}

bool SimpleMesh::InitBuffers(ID3D11Device* device, ProcessedMeshData* mesh)
{
	D3D11_SUBRESOURCE_DATA vertData, indexData;
	HRESULT result;

	D3D11_BUFFER_DESC* vertBufferDesc = new D3D11_BUFFER_DESC;
	D3D11_BUFFER_DESC* indexBufferDesc = new D3D11_BUFFER_DESC;

	mVertCount = mesh->numVerts;
	mIndexCount = mesh->numIndices;

	
	mVertCount = mesh->numVerts;
	mIndexCount = mesh->numIndices;

	vertBufferDesc->Usage = D3D11_USAGE_DEFAULT;
	vertBufferDesc->ByteWidth = sizeof(VertexType) * mVertCount;
	vertBufferDesc->BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertBufferDesc->CPUAccessFlags = 0;
	vertBufferDesc->MiscFlags = 0;
	vertBufferDesc->StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertData.pSysMem = &mesh->vertices[0];
	vertData.SysMemPitch = 0;
	vertData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(vertBufferDesc, &vertData, &mVertBuffer);
	if (FAILED(result))
	{
		return false;
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

	return true;
}

bool SimpleMesh::InitTextures(ID3D11Device* device, ProcessedMeshData* mesh)
{
	/*unsigned int numTextures = mesh->numTextures;
	mTexture = new Texture[numTextures];

	for (int i = 0; i < numTextures; i++)
	{
		string name = mesh->textureNames[i];
		wstring wName(name.begin(), name.end());

		if (!mTexture[i].Init(device, &wName[0]))
			return false;
	}*/

	return true;
}

void SimpleMesh::Render(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &mVertBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//now we're set up, let's render!
	deviceContext->DrawIndexed(mIndexCount, 0, 0);
	
	return;
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
		//delete mVertBuffer;
	}

	if (mTexture)
	{
		mTexture->Shutdown();
		delete [] mTexture;
	}

	return;
}