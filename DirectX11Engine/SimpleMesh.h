//SimpleMesh.h
#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Texture.h"

using namespace DirectX;

class SimpleMesh
{
public:
	SimpleMesh(): mVertBuffer(nullptr), mIndexBuffer(nullptr), mTexture(nullptr) {};
	~SimpleMesh() {};
	SimpleMesh(const SimpleMesh&) = delete;
	SimpleMesh(const SimpleMesh&&) = delete;

	bool Init(ID3D11Device* device, WCHAR* meshName);
	void Shutdown();
	void Render(ID3D11DeviceContext* context);

	int GetIndexCount(int index) const { return mIndexCount[index]; }
	ID3D11ShaderResourceView* GetTexture() { return mTexture->GetTexture(); }

private:
	bool InitBuffers(ID3D11Device* device, WCHAR* meshName);
	void ShutdownBuffers();

	ID3D11Buffer** mVertBuffer;
	ID3D11Buffer** mIndexBuffer;
	Texture* mTexture;
	int* mVertCount;
	int* mIndexCount;
	int mNumMeshes;
};
