//PointSprite.h - a class representing point sprites for particle systems
// A simple container for vert/index buffers (really simple ones for points)
// plus an array of world space positions
#pragma once
#include <DirectXMath.h>

struct ID3D11Buffer;
struct ResourceAllocator;

using namespace DirectX;

class PointSprite
{
public:
	PointSprite(unsigned int numSprites, XMFLOAT3* positions) : mNumSprites(numSprites), mPositions(positions) {};
	~PointSprite() { Shutdown(); }

	bool Init(ResourceAllocator* resourceAllocator);
	void Shutdown();

	unsigned int GetNumberSprites() const { return mNumSprites; }
	const XMFLOAT3* GetPositions() const { return mPositions; }

	ID3D11Buffer* GetVertBuffer() { return mVertexBuffer; }
	ID3D11Buffer* GetUVBuffer() { return mUVBuffer; }
	ID3D11Buffer* GetIndexBuffer() { return mIndexBuffer; }
	ID3D11Buffer* GetInstanceBuffer() { return mInstanceBuffer; }

	void Update();
private:
	ID3D11Buffer* mVertexBuffer = nullptr;
	ID3D11Buffer* mUVBuffer = nullptr;
	ID3D11Buffer* mIndexBuffer = nullptr;
	ID3D11Buffer* mInstanceBuffer = nullptr;

	XMFLOAT3* mPositions;

	unsigned int mNumSprites;
};