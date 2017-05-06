#include <D3D11.h>
#include <fstream>
#include <atlbase.h>
#include <D3DX11async.h>
#include <d3dcompiler.h>
#include "ComputeShader.h"

bool ComputeShader::Init(ID3D11Device* device, HWND hwnd, LPCWSTR csFilename, void* inData, unsigned int numElems)
{
	HRESULT result;

	ID3D10Blob* errorMessage;
	ID3D10Blob* computeShaderBuffer;

	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	computeShaderBuffer = 0;

	result = D3DReadFileToBlob(csFilename, &computeShaderBuffer);

	if (result != S_OK)
		return false;

	result = device->CreateComputeShader(computeShaderBuffer->GetBufferPointer(), computeShaderBuffer->GetBufferSize(), NULL, &mComputeShader);

	if (result != S_OK)
		return false;

	computeShaderBuffer->Release();

	//create srv and uav for storing and releasing info
	D3D11_BUFFER_DESC srvBuffer, uavBuffer;
	ZeroMemory(&srvBuffer, sizeof(srvBuffer));
	srvBuffer.ByteWidth = sizeof(float) * numElems;
	srvBuffer.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	srvBuffer.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	srvBuffer.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	srvBuffer.StructureByteStride = 4;


	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = inData;
	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&srvBuffer, &InitData, &mInData);
	if (result != S_OK)
		return false;


	//create srv
	D3D11_BUFFER_DESC descBuf;
	ZeroMemory(&descBuf, sizeof(descBuf));
	mInData->GetDesc(&descBuf);

	D3D11_SHADER_RESOURCE_VIEW_DESC descView;
	ZeroMemory(&descView, sizeof(descView));
	descView.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	descView.BufferEx.FirstElement = 0;

	descView.Format = DXGI_FORMAT_UNKNOWN;
	descView.BufferEx.NumElements = descBuf.ByteWidth / descBuf.StructureByteStride;

	result = device->CreateShaderResourceView(mInData, &descView, &mSrvIn);

	if (result != S_OK)
		return false;

	ZeroMemory(&uavBuffer, sizeof(uavBuffer));
	uavBuffer.ByteWidth = sizeof(float) * numElems;
	uavBuffer.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	uavBuffer.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	uavBuffer.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	uavBuffer.StructureByteStride = 4;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&uavBuffer, NULL, &mOutData);

	if (result != S_OK)
		return false;

	ZeroMemory(&descBuf, sizeof(descBuf));
	mOutData->GetDesc(&descBuf);
	D3D11_UNORDERED_ACCESS_VIEW_DESC descView2;
	ZeroMemory(&descView2, sizeof(descView2));
	descView2.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	descView2.Buffer.FirstElement = 0;

	descView2.Format = DXGI_FORMAT_UNKNOWN;
	descView2.Buffer.NumElements = descBuf.ByteWidth / descBuf.StructureByteStride;

	result = device->CreateUnorderedAccessView(mOutData, &descView2, &mSrvOut);

	if (result != S_OK)
		return false;

	return true;
}

void ComputeShader::Shutdown()
{
	if (mComputeShader)
		mComputeShader->Release();

	if (mInData)
		mInData->Release();

	if (mOutData)
		mOutData->Release();

}


bool ComputeShader::PrepareShader(ID3D11DeviceContext* deviceContext)
{   
	deviceContext->CSSetShader(mComputeShader , NULL, 0);
	deviceContext->CSSetShaderResources(0, 1, &mSrvIn);
	deviceContext->CSSetUnorderedAccessViews(0, 1, &mSrvOut, NULL);
	return true;
}

bool ComputeShader::CheckContents(ID3D11DeviceContext* deviceContext)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result = deviceContext->Map(mOutData, 0, D3D11_MAP_READ, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	float* data = (float*)mappedResource.pData;

	for (int i = 0; i < 100; i++)
	{
		float num = data[i];
	}

	return true;
}