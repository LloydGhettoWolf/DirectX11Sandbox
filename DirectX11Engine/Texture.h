#pragma once
//Texture.h
#include <d3d11.h>
#include <D3DX11tex.h>

class Texture
{
public:
	Texture() : mTexture(nullptr) {};
	~Texture() {}
	Texture(const Texture&) = delete;

	bool Init(ID3D11Device* device, WCHAR* fileName);
	void Shutdown();
	ID3D11ShaderResourceView* GetTexture() { return mTexture; }
private:
	ID3D11ShaderResourceView* mTexture;
};