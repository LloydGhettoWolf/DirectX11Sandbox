//ColorShader.h
#include <D3D11.h>
#include "SimpleFullScreenShader.h"
#include "ObjParser.h"

#include "Defines.h"


bool SimpleFullScreenShader::Init(ID3D11Device* device, HWND hwnd, unsigned int numLights)
{
	LPCWSTR vsName(L"C://Users/GhettoFett/Documents/Visual Studio 2015/Projects/DirectX11Engine/Debug/SimpleFullScreenVertexShader.cso");
	LPCWSTR psName(L"C://Users/GhettoFett/Documents/Visual Studio 2015/Projects/DirectX11Engine/Debug/SimpleFullScreenPixelShader.cso");

	return InitSimpleFullScreenShader(device, hwnd, &vsName, &psName);
}

bool SimpleFullScreenShader::InitSimpleFullScreenShader(ID3D11Device* device, HWND hwnd, LPCWSTR* vsName, LPCWSTR* psName)
{
	HRESULT result = Shader::InitShaderDataNoLayout(device, hwnd, *vsName, *psName);

	if (!result)
	{
		return false;
	}

	return true;
}

void SimpleFullScreenShader::Shutdown()
{
	Shader::Shutdown();
}


bool SimpleFullScreenShader::PrepareShader(ID3D11DeviceContext* deviceContext)
{
	Shader::PrepareShader(deviceContext);
	return true;
}


bool SimpleFullScreenShader::SetConstantShaderParameters(void* data, ID3D11DeviceContext* deviceContext)
{
	return true;
}

bool SimpleFullScreenShader::SetPerMeshParameters(void* data, ID3D11DeviceContext* deviceContext)
{
	//unpack data
	ShaderPerMeshStruct* info = static_cast<ShaderPerMeshStruct*>(data);
	ID3D11SamplerState* samplerState = info->sampler;
	ID3D11ShaderResourceView* textureSrv = info->diffuseSrv;

	ID3D11ShaderResourceView * textures[] = { textureSrv};
	deviceContext->PSSetShaderResources(0, 1, textures);
	return true;
}

