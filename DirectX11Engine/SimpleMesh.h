//SimpleMesh.h
#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class SimpleMesh
{
public:
	SimpleMesh(): mVertBuffer(nullptr), mIndexBuffer(nullptr) {};
	~SimpleMesh() {};
	SimpleMesh(const SimpleMesh&) = delete;
	SimpleMesh(const SimpleMesh&&) = delete;

	bool Init(ID3D11Device* device);
	void Shutdown();
	void Render(ID3D11DeviceContext* context);

	int GetIndexCount() const { return mIndexCount; }

private:
	bool InitBuffers(ID3D11Device* device);
	void ShutdownBuffers();

	ID3D11Buffer* mVertBuffer;
	ID3D11Buffer* mIndexBuffer;
	int mVertCount;
	int mIndexCount;

	struct VertexType
	{
		XMVECTOR position;
		XMVECTOR color;
	};
};
