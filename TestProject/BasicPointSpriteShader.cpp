#include "BasicPointSpriteShader.h"
#include "Defines.h"
#include <D3D11.h>
#include <DirectXMath.h>

bool PSShader::Init(ID3D11Device* device, HWND hwnd, unsigned int numLights)
{
	D3D11_BUFFER_DESC bufferDesc;

	D3D11_INPUT_ELEMENT_DESC polygonLayout[4];

	Shader::CreateInstancedPosLayout(polygonLayout);

	LPCWSTR vsName(L"C://Users/GhettoFett/Documents/Visual Studio 2015/Projects/DirectX11Engine/Debug/SimpleInstanceVertexShader.cso");
	LPCWSTR psName(L"C://Users/GhettoFett/Documents/Visual Studio 2015/Projects/DirectX11Engine/Debug/SimpleInstancePixelShader.cso");
	bool bResult = Shader::InitShaderData(device, hwnd, vsName, psName, polygonLayout, 4);
	if (!bResult)
	{
		return false;
	}

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(MatrixBufferType);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	HRESULT result = device->CreateBuffer(&bufferDesc, NULL, &mMatrixBuffer);

	if (result != S_OK)
	{
		return false;
	}

	bufferDesc.ByteWidth = sizeof(EyeBufferType);
	result = device->CreateBuffer(&bufferDesc, NULL, &mEyeBuffer);

	if (result != S_OK)
	{
		return false;
	}

	return true;
}

void PSShader::Shutdown()
{
	Shader::Shutdown();
}

bool PSShader::PrepareShader(ID3D11DeviceContext* deviceContext)
{
	Shader::PrepareShader(deviceContext);
	return true;
}

bool PSShader::SetConstantShaderParameters(void* data, ID3D11DeviceContext* deviceContext)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	D3D11_MAPPED_SUBRESOURCE mappedResource2;

	MatrixBufferType* matPtr;

	InstancedConstantsStruct* constants = static_cast<InstancedConstantsStruct*>(data);
	MatrixBufferType* matrices = constants->matPtr;

	// Lock the constant buffer so it can be written to.
	HRESULT result = deviceContext->Map(mMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	matPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	matPtr->world = matrices->world;
	matPtr->view = matrices->view;
	matPtr->projection = matrices->projection;

	// Unlock the constant buffer.
	deviceContext->Unmap(mMatrixBuffer, 0);

	EyeBufferType* extEyePtr = constants->eyePtr;
	result = deviceContext->Map(mEyeBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource2);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	EyeBufferType* eyePtr = (EyeBufferType*)mappedResource2.pData;

	// Copy the matrices into the constant buffer.
	eyePtr->eyePos = extEyePtr->eyePos;
	
	// Unlock the constant buffer.
	deviceContext->Unmap(mEyeBuffer, 0);

	// Finanly set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(EYE_BUFFER, 1, &mEyeBuffer);
	return true;
}

bool PSShader::SetPerMeshParameters(void* data, ID3D11DeviceContext* deviceContext)
{	

	ShaderPerMeshStruct* info = static_cast<ShaderPerMeshStruct*>(data);
	ID3D11ShaderResourceView* srv = info->diffuseSrv;
	ID3D11SamplerState* samplerState = info->sampler;

	deviceContext->PSSetSamplers(0, 1, &samplerState);
	deviceContext->PSSetShaderResources(0, 1, &srv);
	return true;
}