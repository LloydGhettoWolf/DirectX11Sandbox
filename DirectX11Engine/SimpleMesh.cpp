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
	D3D11_SUBRESOURCE_DATA vertData, indexData;
	HRESULT result;

	wstring name(meshName);
	string strName(name.begin(), name.end());
	ProcessedMeshData* mesh = ReadBoomFile(strName);

	mNumMeshes = mesh->numMeshes;

	D3D11_BUFFER_DESC* vertBufferDesc = new D3D11_BUFFER_DESC[mNumMeshes];
	D3D11_BUFFER_DESC* indexBufferDesc = new D3D11_BUFFER_DESC[mNumMeshes];

	mVertCount = new int[mNumMeshes];
	mIndexCount = new int[mNumMeshes];

	mVertBuffer = new ID3D11Buffer*[mNumMeshes];
	mIndexBuffer = new ID3D11Buffer*[mNumMeshes];

	for (int i = 0; i < mNumMeshes; i++)
	{
		mVertCount[i] = mesh->numVerts[i];
		mIndexCount[i] = mesh->numIndices[i];

		vertBufferDesc[i].Usage = D3D11_USAGE_DEFAULT;
		vertBufferDesc[i].ByteWidth = sizeof(VertexType) * mVertCount[i];
		vertBufferDesc[i].BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertBufferDesc[i].CPUAccessFlags = 0;
		vertBufferDesc[i].MiscFlags = 0;
		vertBufferDesc[i].StructureByteStride = 0;

		// Give the subresource structure a pointer to the vertex data.
		vertData.pSysMem = &mesh->vertices[i][0];
		vertData.SysMemPitch = 0;
		vertData.SysMemSlicePitch = 0;

		// Now create the vertex buffer.
		result = device->CreateBuffer(&vertBufferDesc[i], &vertData, &mVertBuffer[i]);
		if (FAILED(result))
		{
			return false;
		}

		// Set up the description of the static index buffer.
		indexBufferDesc[i].Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc[i].ByteWidth = sizeof(unsigned int) * mIndexCount[i];
		indexBufferDesc[i].BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc[i].CPUAccessFlags = 0;
		indexBufferDesc[i].MiscFlags = 0;
		indexBufferDesc[i].StructureByteStride = 0;

		// Give the subresource structure a pointer to the index data.
		indexData.pSysMem = &mesh->indices[i][0];
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		// Create the index buffer.
		result = device->CreateBuffer(&indexBufferDesc[i], &indexData, &mIndexBuffer[i]);
		if (FAILED(result))
		{
			return false;
		}
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

	for (int i = 0; i < mNumMeshes; i++)
	{
		// Set the vertex buffer to active in the input assembler so it can be rendered.
		deviceContext->IASetVertexBuffers(0, 1, &mVertBuffer[i], &stride, &offset);

		// Set the index buffer to active in the input assembler so it can be rendered.
		deviceContext->IASetIndexBuffer(mIndexBuffer[i], DXGI_FORMAT_R32_UINT, 0);

		// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//now we're set up, let's render!
		deviceContext->DrawIndexed(mIndexCount[i], 0, 0);
	}

	return;
}

void SimpleMesh::ShutdownBuffers()
{
	// Release the index buffer.
	if (mIndexBuffer)
	{
		for (int i = 0; i < mNumMeshes; i++)
			mIndexBuffer[i]->Release();

		delete mIndexBuffer;
	}

	// Release the vertex buffer.
	if (mVertBuffer)
	{
		for (int i = 0; i < mNumMeshes; i++)
			mVertBuffer[i]->Release();

		delete mVertBuffer;
	}

	if (mTexture)
	{
		mTexture->Shutdown();
		delete mTexture;
	}

	return;
}