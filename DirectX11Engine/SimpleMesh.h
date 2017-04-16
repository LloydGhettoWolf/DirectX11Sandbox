//SimpleMesh.h
#pragma once
#include <DirectXMath.h>
using namespace DirectX;

struct ID3D11Buffer;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ProcessedMeshData;
struct ProcessedNormalMappedMeshData;
class ResourceAllocator;

class SimpleMesh
{
public:
	SimpleMesh(): mVertBuffer(nullptr), mIndexBuffer(nullptr) {};
	~SimpleMesh() { Shutdown(); }
	SimpleMesh(const SimpleMesh&) = delete;
	SimpleMesh(const SimpleMesh&&) = delete;

	bool Init(ResourceAllocator* resourceAllocator, ProcessedMeshData* mesh);
	void Shutdown();

	unsigned int GetIndexCount( ) const { return mIndexCount; }
	unsigned int GetMaterialIndex() const { return mMaterialIndex; }
	unsigned int GetNumBuffers() const { return mNumBuffers; }
	const unsigned int* GetStrides() const  { return mStrides; }
	bool GetIsMapped() const { return mIsNormalMapped; }
	
	ID3D11Buffer** GetVertBuffer()  { return mVertBuffer; }
	ID3D11Buffer* GetIndexBuffer()  { return mIndexBuffer; }

	ID3D11Buffer* GetBoundingBoxVertBuffer() { return mAABBVertBuffer; }
	ID3D11Buffer* GetBoundingBoxIndexBuffer() { return mAABBIndexBuffer; }

	XMFLOAT3 GetBoxMin() const { return min; }
	XMFLOAT3 GetBoxMax() const { return max; }
	XMFLOAT3 GetBoxCenter() const { return center; }

	void SetBoxMin(XMFLOAT3& newMin) { min = newMin; }
	void SetBoxMax(XMFLOAT3& newMax) { max = newMax; }
	void SetBoxCenter(XMFLOAT3& newCenter) { center = newCenter; }

	float thisFrameZ;
private:
	bool InitBoundingBox(ID3D11Device* device);
	bool InitBuffers(ResourceAllocator* resourceAllocator, ProcessedMeshData* mesh);

	ID3D11Buffer** mVertBuffer = nullptr;
	ID3D11Buffer* mIndexBuffer = nullptr;

	ID3D11Buffer* mAABBVertBuffer = nullptr;
	ID3D11Buffer* mAABBIndexBuffer = nullptr;

	unsigned int mMaterialIndex;
	unsigned int mVertCount;
	unsigned int mIndexCount;
	unsigned int mNumBuffers;
	unsigned int mStrides[10];

	bool mIsNormalMapped = false;

	XMFLOAT3 min;
	XMFLOAT3 max;
	XMFLOAT3 center;
	
};