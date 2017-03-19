//Texture.cpp
#include <DirectXTex.h>
#include <D3D11.h>
#include "Texture.h"

using namespace DirectX;

bool Texture::Init(ID3D11DeviceContext* context, ID3D11Device* device, WCHAR* fileName, bool needMips)
{
	ScratchImage image;
	HRESULT result = LoadFromTGAFile(fileName, nullptr, image);

	if (needMips)
	{
		D3D11_TEXTURE2D_DESC texDesc = { 0 };
		texDesc.Height = image.GetMetadata().height;
		texDesc.Width = image.GetMetadata().width; 
		texDesc.MipLevels = 0;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		ID3D11Texture2D* mipTexture = nullptr;

		result = device->CreateTexture2D(&texDesc, nullptr, &mipTexture);

		if (result != S_OK)
			return false;
	
		UINT width = image.GetMetadata().width;
		context->UpdateSubresource(mipTexture, 0, nullptr, (void*)image.GetPixels(), width * 4, 0);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

		// Setup the shader resource view description.
		srvDesc.Format = texDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = -1;

		/* snippet removed, obvious SRV creation, same mip levels, same format */
		result = device->CreateShaderResourceView(mipTexture, &srvDesc, &mTexture);
		if (result != S_OK)
			return false;

		context->GenerateMips(mTexture);

	}
	else
	{
		result = CreateShaderResourceView(device, image.GetImage(0,0,0), image.GetImageCount(), image.GetMetadata(), &mTexture);
	}

	return true;
}

void Texture::Shutdown()
{
	if (mTexture)
	{
		mTexture->Release();
	}
}