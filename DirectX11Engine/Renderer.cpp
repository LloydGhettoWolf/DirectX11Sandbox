

#include <d3d11.h>
#include "Renderer.h"


Renderer::Renderer(ID3D11Device* device, ID3D11DeviceContext* context) : mDevice(device), mDeviceContext(context)  {};

void Renderer::OpaquePassSimpleMesh(SimpleMesh* meshes)
{

}

void Renderer::AlphaPassSimpleMesh(SimpleMesh* meshes)
{

}

void Renderer::DrawIndexed(ID3D11Buffer* const* vertexInfoStreams, 
						   unsigned int numStreams, 
						   const unsigned int* strides,
						   ID3D11Buffer* indices, 
						   unsigned int numIndices,
						   D3D11_PRIMITIVE_TOPOLOGY primitiveType)
{
	unsigned int* offsets = new unsigned int[numStreams];
	for (int i = 0; i < numStreams; i++)
	{
		offsets[i] = 0;
	}
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	mDeviceContext->IASetVertexBuffers(0, numStreams, &vertexInfoStreams[0], &strides[0], &offsets[0]);
		
	// Set the index buffer to active in the input assembler so it can be rendered.
	mDeviceContext->IASetIndexBuffer(indices, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	mDeviceContext->IASetPrimitiveTopology(primitiveType);

	//now we're set up, let's render!
	mDeviceContext->DrawIndexed(numIndices, 0, 0);
}

void Renderer::DrawIndexedInstanced(ID3D11Buffer* const* vertexInfoStreams,
	unsigned int numStreams,
	const unsigned int* strides,
	ID3D11Buffer* indices,
	unsigned int numIndices,
	D3D11_PRIMITIVE_TOPOLOGY primitiveType,
	unsigned int numInstances)
{
	unsigned int* offsets = new unsigned int[numStreams];

	for (int i = 0; i < numStreams; i++)
	{
		offsets[i] = 0;
	}

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	mDeviceContext->IASetVertexBuffers(0, numStreams, &vertexInfoStreams[0], &strides[0], &offsets[0]);

	// Set the index buffer to active in the input assembler so it can be rendered.
	mDeviceContext->IASetIndexBuffer(indices, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	mDeviceContext->IASetPrimitiveTopology(primitiveType);

	//now we're set up, let's render!
	mDeviceContext->DrawIndexedInstanced(numIndices, numInstances, 0, 0, 0);
}