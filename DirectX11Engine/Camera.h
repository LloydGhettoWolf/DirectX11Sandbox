#pragma once
#include <DirectXMath.h>
using namespace DirectX;

const float DEGS_PER_SEC = 60.0f;

class Camera 
{
	public:
		Camera()  {};
		~Camera() {};

		bool   Init(XMVECTOR& pos, XMVECTOR& lookAt, XMVECTOR& up, float nearZ, float farZ, int width, int height, float fov);
		void   MoveCameraForward(float amount);
		void   MoveCameraHorizontally(float amount);
		void   MoveCameraVertically(float amount);
		void   ComboRotate(float amountX, float amountY);

		const XMFLOAT4X4& GetView() const { return mViewMatrix; }
		const XMFLOAT4X4& GetProjection() const { return mProjectionMatrix; }

		const XMFLOAT3& GetPos() const { return mPos; }

	private:
		XMFLOAT3 mLookVec;
		XMFLOAT3 mUpVec;
		XMFLOAT3 mRightVec;
		XMFLOAT3 mPos;

		XMFLOAT4X4 mViewMatrix;
		XMFLOAT4X4 mProjectionMatrix;

		float yRotation = -3.14129f / 4.0f;
};

