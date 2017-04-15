//ColorShader.h
#include <D3D11.h>
#include <DirectXMath.h>
#include "DefaultDiffuseShader.h"
#include "ObjParser.h"


bool DefaultDiffuseShader::Init(ID3D11Device* device, HWND hwnd)
{
	D3D11_BUFFER_DESC bufferDesc;

	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];

	Shader::CreatePosNormUvLayout(polygonLayout);

	LPCWSTR vsName(L"C://Users/GhettoFett/Documents/Visual Studio 2015/Projects/DirectX11Engine/Debug/SimpleVertexShader.cso");
	LPCWSTR psName(L"C://Users/GhettoFett/Documents/Visual Studio 2015/Projects/DirectX11Engine/Debug/SimplePixelShader.cso");

	HRESULT result = Shader::InitShaderData(device, hwnd, vsName, psName, polygonLayout, 3);

	if (result != true)
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
	result = device->CreateBuffer(&bufferDesc, NULL, &mMatrixBuffer);

	if (result != S_OK)
	{
		return false;
	}

	bufferDesc.ByteWidth = sizeof(LightPosBuffer);

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&bufferDesc, NULL, &mLightBuffer);

	if (result != S_OK)
	{
		return false;
	}

	bufferDesc.ByteWidth = sizeof(MaterialProperties);

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&bufferDesc, NULL, &mMaterialBuffer);

	if (result != S_OK)
	{
		return false;
	}

	return true;
}

void DefaultDiffuseShader::Shutdown()
{
	Shader::Shutdown();

	// Release the matrix constant buffer.
	if (mMatrixBuffer)
	{
		mMatrixBuffer->Release();
	}

	if (mLightBuffer)
	{
		mLightBuffer->Release();
	}

	if (mMaterialBuffer)
	{
		mMaterialBuffer->Release();
	}

	return;
}


bool DefaultDiffuseShader::PrepareShader(ID3D11DeviceContext* deviceContext)
{
	Shader::PrepareShader(deviceContext);
	return true;
}


bool DefaultDiffuseShader::SetConstantShaderParameters(void* data, ID3D11DeviceContext* deviceContext)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	D3D11_MAPPED_SUBRESOURCE mappedResource2;
	
	MatrixBufferType* matPtr;
	LightPosBuffer* lightPtr;

	ConstantsStruct* constants = static_cast<ConstantsStruct*>(data);
	MatrixBufferType* matrices = constants->matPtr;
	LightPosBuffer* lights = constants->lightPtr;

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

	result = deviceContext->Map(mLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource2);
	if (FAILED(result))
	{
		return false;
	}

	lightPtr = (LightPosBuffer*)mappedResource2.pData;
	lightPtr->lightPos = lights->lightPos;
	lightPtr->lightCol = lights->lightCol;

	// Unlock the constant buffer.
	deviceContext->Unmap(mLightBuffer, 0);


	// Finanly set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(MATRIX_BUFFER, 1, &mMatrixBuffer);
	deviceContext->PSSetConstantBuffers(LIGHT_BUFFER, 1, &mLightBuffer);

	return true;
}

bool DefaultDiffuseShader::SetPerMeshParameters(void* data, ID3D11DeviceContext* deviceContext) 
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MaterialProperties* meshPtr3;

	//unpack data
	ShaderPerMeshStruct* info = static_cast<ShaderPerMeshStruct*>(data);
	MaterialProperties* matInfo = info->material;
	ID3D11SamplerState* samplerState = info->sampler;
	ID3D11ShaderResourceView* srv = info->diffuseSrv;

	HRESULT result = deviceContext->Map(mMaterialBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}
	
	meshPtr3 = (MaterialProperties*)mappedResource.pData;
	meshPtr3->diffuseCol = matInfo->diffuseCol;
	meshPtr3->specCol = matInfo->specCol;
	meshPtr3->specComponent = matInfo->specComponent;

	deviceContext->Unmap(mMaterialBuffer, 0);

	deviceContext->PSSetConstantBuffers(MATERIAL_BUFFER, 1, &mMaterialBuffer);
	deviceContext->PSSetSamplers(0, 1, &samplerState);
	deviceContext->PSSetShaderResources(0, 1, &srv);
}

