#pragma once
//Texture.h
#include <esent.h>

struct TargaHeader
{
	unsigned char data1[12];
	unsigned short width;
	unsigned short height;
	unsigned char bpp;
	unsigned char data2;
};

struct ID3D11DeviceContext;
struct ID3D11Device;
struct ID3D11ShaderResourceView;

class Texture
{
public:
	Texture() : mTexture(nullptr) {};
	~Texture() {}
	Texture(const Texture&) = delete;

	bool Init(ID3D11DeviceContext* context, ID3D11Device* device, WCHAR* fileName, bool needMips = false);
	bool InitFromDDS(ID3D11Device* device, WCHAR* fileName);
	void Shutdown();
	ID3D11ShaderResourceView* GetTexture() { return mTexture; }
private:
	ID3D11ShaderResourceView* mTexture;
};