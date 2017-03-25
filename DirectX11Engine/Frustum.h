#pragma once
//frustum.h
#include <DirectXMath.h>

using namespace DirectX;

class Camera;
struct ID3D11Buffer;

class Frustum
{
	public:
		Frustum() {};
		~Frustum() {};

		bool InitFrustum(ID3D11Device* device);
		bool UpdateFrustum(Camera* cam);
		bool RenderFrustum(ID3D11DeviceContext* deviceContext);
		bool CheckBox(XMFLOAT4& min, XMFLOAT4& max);
	private:
		ID3D11Buffer* mVertBuffer;
		ID3D11Buffer* mIndexBuffer;

		XMMATRIX mFrustum;
		float mPlanes[6][4];
};
