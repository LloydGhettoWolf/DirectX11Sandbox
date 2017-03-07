#pragma once
//Texture.h
#include <d3d11.h>

struct TargaHeader
{
	unsigned char data1[12];
	unsigned short width;
	unsigned short height;
	unsigned char bpp;
	unsigned char data2;
};

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
	bool LoadTarga(char* filename, int& height, int& width);
	ID3D11ShaderResourceView* mTexture;
	unsigned char* m_targaData;
};