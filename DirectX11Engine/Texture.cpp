//Texture.cpp
#include "Texture.h"

bool Texture::Init(ID3D11Device* device, WCHAR* fileName)
{
	HRESULT result = D3DX11CreateShaderResourceViewFromFile(device, fileName, 0, 0, &mTexture, 0);
	if ( result != S_OK)
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