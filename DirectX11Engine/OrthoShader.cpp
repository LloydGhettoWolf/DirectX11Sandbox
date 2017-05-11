#include <D3D11.h>
#include "OrthoShader.h"

bool OrthoShader::Init(ID3D11Device* device, HWND hwnd, unsigned int numLights)
{
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];

	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 1;
	polygonLayout[1].AlignedByteOffset = 0;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

#if _DEBUG 
	LPCWSTR vsName(L"C://Users/GhettoFett/Documents/Visual Studio 2015/Projects/DirectX11Engine/Debug/OrthoVertexShader.cso");
	LPCWSTR psName(L"C://Users/GhettoFett/Documents/Visual Studio 2015/Projects/DirectX11Engine/Debug/OrthoPixelShader.cso");
#else
	LPCWSTR vsName(L"C://Users/GhettoFett/Documents/Visual Studio 2015/Projects/DirectX11Engine/Release/OrthoVertexShader.cso");
	LPCWSTR psName(L"C://Users/GhettoFett/Documents/Visual Studio 2015/Projects/DirectX11Engine/Release/OrthoPixelShader.cso");
#endif

	Shader::InitShaderData(device, hwnd, vsName, psName, polygonLayout, 1);
	D3D11_BUFFER_DESC matrixBufferDesc;

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	HRESULT result = device->CreateBuffer(&matrixBufferDesc, NULL, &mMatrixBuffer);

	if (result != S_OK)
	{
		return false;
	}

	return true;
}

void OrthoShader::Shutdown()
{
	Shader::Shutdown();

	// Release the matrix constant buffer.
	if (mMatrixBuffer)
	{
		mMatrixBuffer->Release();
	}

}

bool OrthoShader::PrepareShader(ID3D11DeviceContext* deviceContext)
{
	Shader::PrepareShader(deviceContext);
	return true;
}

bool OrthoShader::SetConstantShaderParameters(void* data, ID3D11DeviceContext* deviceContext)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	MatrixBufferType* matPtr;
	MatrixBufferType* matrices = (MatrixBufferType*)data;

	// Lock the constant buffer so it can be written to.
	HRESULT result = deviceContext->Map(mMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	matPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	matPtr->orthoProjection = matrices->orthoProjection;

	// Unlock the constant buffer.
	deviceContext->Unmap(mMatrixBuffer, 0);

	deviceContext->VSSetConstantBuffers(0, 1, &mMatrixBuffer);

	return true;
}