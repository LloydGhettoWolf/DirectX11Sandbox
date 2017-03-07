//Texture.cpp
#include <DirectXTex.h>
#include "Texture.h"

using namespace DirectX;

bool Texture::Init(ID3D11Device* device, WCHAR* fileName)
{
	//HRESULT result = D3DX11CreateShaderResourceViewFromFile(device, fileName, 0, 0, &mTexture, 0);

	ScratchImage image;
	HRESULT result = LoadFromTGAFile(fileName, nullptr, image);
	
	if ( result != S_OK)
		return false;


	result = CreateShaderResourceView(device, image.GetImages(), 1, image.GetMetadata(), &mTexture);

	if (result != S_OK)
		return false;

	return true;
}

void Texture::Shutdown()
{
	if (mTexture)
	{
		mTexture->Release();
	}
}