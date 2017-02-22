//simpleMesh.cpp
#include "SimpleMesh.h"
#include "ObjParser.h"

bool SimpleMesh::Init(ID3D11Device* device, WCHAR* meshName)
{
	if (!InitBuffers(device, meshName))
		return false;

	//mTexture = new Texture;

	//if (!mTexture->Init(device, fileName))
		//return false;

	return true;
}

void SimpleMesh::Shutdown() {
	ShutdownBuffers();
}

bool SimpleMesh::InitBuffers(ID3D11Device* device, WCHAR* meshName)
{

	D3D11_BUFFER_DESC vertBufferDesc;
	D3D11_BUFFER_DESC indexBufferDesc;

	D3D11_SUBRESOURCE_DATA vertData, indexData;
	HRESULT result;

	wstring name(meshName);
	string strName(name.begin(), name.end());
	ProcessedMeshData* mesh = ReadBoomFile(strName);

	mVertCount = mesh->numVerts;
	mIndexCount = mesh->numIndices;


	vertBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertBufferDesc.ByteWidth = sizeof(VertexType) * mVertCount;
	vertBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertBufferDesc.CPUAccessFlags = 0;
	vertBufferDesc.MiscFlags = 0;
	vertBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertData.pSysMem = &mesh->vertices[0];
	vertData.SysMemPitch = 0;
	vertData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertBufferDesc, &vertData, &mVertBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * mIndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = &mesh->indices[0];
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &mIndexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	delete mesh;

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
	}

	// Release the vertex buffer.
	if (mVertBuffer)
	{
		mVertBuffer->Release();
	}

	if (mTexture)
	{
		mTexture->Shutdown();
		delete mTexture;
	}

	return;
}