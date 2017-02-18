//simpleMesh.cpp
#include "SimpleMesh.h"

bool SimpleMesh::Init(ID3D11Device* device)
{
	bool result = InitBuffers(device);

	return result;
}

void SimpleMesh::Shutdown() {
	ShutdownBuffers();
}

bool SimpleMesh::InitBuffers(ID3D11Device* device)
{
	VertexType* verts;
	unsigned long* indices;

	D3D11_BUFFER_DESC vertBufferDesc;
	D3D11_BUFFER_DESC indexBufferDesc;

	D3D11_SUBRESOURCE_DATA vertData, indexData;
	HRESULT result;

	mVertCount = 4;
	mIndexCount = 6;

	verts = new VertexType[mVertCount];
	indices = new unsigned long[mIndexCount];

	verts[0].position = XMVectorSet(-1.0f, -1.0f, 0.0f, 1.0f);  // Bottom left.
	verts[0].color = XMVectorSet(1.0f, 1.0f, 0.0f, 1.0f);

	verts[1].position = XMVectorSet(-1.0f, 1.0f, 0.0f, 1.0f);  // Top middle.
	verts[1].color = XMVectorSet(0.0f, 1.0f, 1.0f, 1.0f);

	verts[2].position = XMVectorSet(1.0f, -1.0f, 0.0f, 1.0f);  // Bottom right.
	verts[2].color = XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f);

	verts[3].position = XMVectorSet(1.0f, 1.0f, 0.0f, 1.0f);  // top right.
	verts[3].color = XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 2;
	indices[4] = 1;
	indices[5] = 3;

	vertBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertBufferDesc.ByteWidth = sizeof(VertexType) * mVertCount;
	vertBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertBufferDesc.CPUAccessFlags = 0;
	vertBufferDesc.MiscFlags = 0;
	vertBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertData.pSysMem = verts;
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
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * mIndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &mIndexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	delete [] verts;
	delete[] indices;

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

	return;
}