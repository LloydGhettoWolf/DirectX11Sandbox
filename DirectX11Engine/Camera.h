#pragma once
#include <DirectXMath.h>
using namespace DirectX;

class Camera 
{
	public:
		Camera()  {};
		~Camera() {};

		bool   Init(XMVECTOR& pos, XMVECTOR& lookAt, XMVECTOR& up, float nearZ, float farZ, int width, int height, float fov);
		void   MoveCameraForward(float amount);
		void   MoveCameraHorizontally(float amount);
		void   MoveCameraVertically(float amount);
		void   MoveCameraCustom(XMVECTOR& pos);
		void   MoveCameraTrackPoint(XMVECTOR& pos, XMVECTOR& focus);
		void   ComboRotate(float amountX, float amountY);

		XMMATRIX GetView() const { return mViewMatrix; }
		XMMATRIX GetProjection() const { return mProjectionMatrix; }

		XMVECTOR GetPos() const { return mPos; }

	private:
		XMVECTOR mLookVec;
		XMVECTOR mUpVec;
		XMVECTOR mRightVec;
		XMVECTOR mPos;

		float yRotation = -3.14129f / 4.0f;

		XMMATRIX mViewMatrix;
		XMMATRIX mProjectionMatrix;
};

