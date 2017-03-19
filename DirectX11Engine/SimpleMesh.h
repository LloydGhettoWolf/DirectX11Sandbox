//SimpleMesh.h
#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Texture.h"
#include "ObjParser.h"
#include "DefaultDiffuseShader.h"

using namespace DirectX;

class SimpleMesh
{
public:
	SimpleMesh(): mVertBuffer(nullptr), mIndexBuffer(nullptr) {};
	~SimpleMesh() {};
	SimpleMesh(const SimpleMesh&) = delete;
	SimpleMesh(const SimpleMesh&&) = delete;

	bool Init(ID3D11Device* device, ProcessedMeshData* mesh);
	void Shutdown();
	void Render(ID3D11DeviceContext* context);

	int GetIndexCount( ) const { return mIndexCount; }
	int GetMaterialIndex() const { return mMaterialIndex; }

	ID3D11Buffer* GetVertBuffer()  { return mVertBuffer; }
	ID3D11Buffer* GetIndexBuffer()  { return mIndexBuffer; }

private:
	bool InitBuffers(ID3D11Device* device, ProcessedMeshData* mesh);
	bool InitTextures(ID3D11Device* device, ProcessedMeshData* mesh);
	void ShutdownBuffers();

	ID3D11Buffer* mVertBuffer;
	ID3D11Buffer* mIndexBuffer;
	DefaultDiffuseShader* shader;
	int mMaterialIndex;
	int mVertCount;
	int mIndexCount;
};
