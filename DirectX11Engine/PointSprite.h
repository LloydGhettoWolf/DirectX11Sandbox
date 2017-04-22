//PointSprite.h - a class representing point sprites for particle systems
// A simple container for vert/index buffers (really simple ones for points)
// plus an array of world space positions
#pragma once
#include <DirectXMath.h>

struct ID3D11Buffer;
struct ResourceAllocator;
struct Texture;

using namespace DirectX;

class PointSprite
{
public:
	PointSprite(unsigned int numSprites, XMFLOAT4* data) : mNumSprites(numSprites), mData(data) {};
	~PointSprite() { Shutdown(); }

	bool Init(ResourceAllocator* resourceAllocator, ID3D11Device* device, WCHAR* fileName);
	void Shutdown();

	unsigned int GetNumberSprites() const { return mNumSprites; }
	const XMFLOAT4* GetPositions() const { return mData; }

	ID3D11Buffer* GetVertBuffer() { return mVertexBuffer; }
	ID3D11Buffer* GetUVBuffer() { return mUVBuffer; }
	ID3D11Buffer* GetIndexBuffer() { return mIndexBuffer; }
	ID3D11Buffer* GetInstancePosBuffer() { return mInstanceBuffer; }

	ID3D11Buffer** GetAllBuffers();

	ID3D11ShaderResourceView* GetTexture();

	bool Update(ID3D11DeviceContext* context, XMFLOAT4* data);
private:
	ID3D11Buffer* mVertexBuffer = nullptr;
	ID3D11Buffer* mUVBuffer = nullptr;
	ID3D11Buffer* mIndexBuffer = nullptr;
	ID3D11Buffer* mInstanceBuffer = nullptr;

	Texture* mPointTexture = nullptr;

	XMFLOAT4* mData;

	unsigned int mNumSprites;
};