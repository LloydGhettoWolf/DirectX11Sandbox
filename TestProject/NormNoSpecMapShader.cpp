//ColorShader.h
#include <D3D11.h>
#include "NormNoSpecMapShader.h"
#include "NormalMapShader.h"
#include "ObjParser.h"

#include "Defines.h"


bool NormNoSpecMapShader::Init(ID3D11Device* device, HWND hwnd)
{
	LPCWSTR vsName(L"C://Users/GhettoFett/Documents/Visual Studio 2015/Projects/DirectX11Engine/Debug/NormNoSpecMapVertexShader.cso");
	LPCWSTR psName(L"C://Users/GhettoFett/Documents/Visual Studio 2015/Projects/DirectX11Engine/Debug/NormNoSpecMapPixelShader.cso");

	return InitNormalMapShader(device, hwnd, &vsName, &psName);
}


bool NormNoSpecMapShader::SetPerMeshParameters(void* data, ID3D11DeviceContext* deviceContext)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MaterialProperties* meshPtr3;

	//unpack data
	NormShaderPerMeshStruct* info = static_cast<NormShaderPerMeshStruct*>(data);
	MaterialProperties* matInfo = info->material;
	ID3D11SamplerState* samplerState = info->sampler;
	ID3D11ShaderResourceView* diffuseSrv = info->diffuseSrv;
	ID3D11ShaderResourceView* specSrv = info->specSrv;
	ID3D11ShaderResourceView* normSrv = info->normSrv;

	HRESULT result = deviceContext->Map(mMaterialBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	meshPtr3 = (MaterialProperties*)mappedResource.pData;
	meshPtr3->diffuseCol = matInfo->diffuseCol;
	meshPtr3->specComponent = matInfo->specComponent;

	deviceContext->Unmap(mMaterialBuffer, 0);

	deviceContext->PSSetConstantBuffers(MATERIAL_BUFFER, 1, &mMaterialBuffer);
	deviceContext->PSSetSamplers(0, 1, &samplerState);

	ID3D11ShaderResourceView * textures[] = { diffuseSrv, normSrv };
	deviceContext->PSSetShaderResources(0, 2, textures);
}

