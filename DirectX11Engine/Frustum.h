#pragma once
//frustum.h
#include <DirectXMath.h>

using namespace DirectX;

class Camera;
struct ID3D11Buffer;
struct ID3D11Device;

class Frustum
{
	public:
		Frustum() {};
		~Frustum() {};

		bool InitFrustum(ID3D11Device* device);
		void UpdateFrustum(Camera* cam);
		void UpdateFrustumFast(Camera* cam);
		bool RenderFrustum(ID3D11DeviceContext* deviceContext);
		bool CheckBox(XMFLOAT3& min, XMFLOAT3& max);
		bool CheckBoxFast(XMFLOAT3& min, XMFLOAT3& max);
	private:
		ID3D11Buffer* mVertBuffer;
		ID3D11Buffer* mIndexBuffer;

		XMFLOAT4X4 mFrustum;
		float mPlanes[6][4];

		XMFLOAT4 mPlanesFast[6];
};
