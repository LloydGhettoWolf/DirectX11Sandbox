//Camera.cpp
#include "Camera.h"
#include <D3D11.h>

bool Camera::Init(XMVECTOR& pos, XMVECTOR& lookAt, XMVECTOR& up, float nearZ, float farZ, int width, int height, float fov)
{
	mViewMatrix = XMMatrixLookAtLH(pos, lookAt, up);

	mLookVec = XMVector3Normalize(lookAt - pos);
	mUpVec = XMVector3Normalize(up);
	mRightVec = XMVector3Cross(mLookVec, mUpVec);
	mRightVec = XMVector3Normalize(mRightVec);

	mUpVec = XMVector3Cross(mRightVec, mLookVec);
	mUpVec = XMVector3Normalize(mUpVec);

	mPos = pos;

	mProjectionMatrix = XMMatrixPerspectiveFovLH(fov, (float)width / (float)height, nearZ, farZ);
	return true;
}

void  Camera::MoveCameraHorizontally(float amount) 
{
	XMVECTOR translationVec = -amount * mRightVec;
	mPos += translationVec;
	mViewMatrix = XMMatrixLookAtLH(mPos, mPos + mLookVec, mUpVec);
}


void  Camera::MoveCameraForward(float amount)
{
	XMVECTOR translationVec = -amount * mLookVec;
	mPos += translationVec;
	mViewMatrix = XMMatrixLookAtLH(mPos, mPos + mLookVec, mUpVec);
}

void  Camera::MoveCameraVertically(float amount)
{
	XMVECTOR translationVec = -amount * mUpVec;
	mPos += translationVec;
	mViewMatrix = XMMatrixLookAtLH(mPos, mPos + mLookVec, mUpVec);
}

void   Camera::MoveCameraCustom(XMVECTOR& pos)
{
	pos = pos;
	mViewMatrix = XMMatrixLookAtLH(mPos, mPos + mLookVec, mUpVec);
}

void   Camera::MoveCameraTrackPoint(XMVECTOR& pos, XMVECTOR& focus)
{
	mViewMatrix = XMMatrixLookAtLH(mPos, focus, mUpVec);
}

void Camera::ComboRotate(float amountX, float amountY)
{
	yRotation += XMConvertToRadians(-amountY);

	if (yRotation > 0.0f)
		yRotation = 0.0f;

	if (yRotation < -3.14129f / 2.0f)
		yRotation = -3.14129f / 2.0f;

	XMMATRIX rotation = XMMatrixRotationY(amountX);
	mLookVec = XMVector3Transform(mLookVec, rotation);
	
	mLookVec = XMVector3Normalize(mLookVec);

	
	rotation = XMMatrixRotationAxis(mRightVec, -amountY);
	mLookVec = XMVector3Transform(mLookVec, rotation);
	mLookVec = XMVector3Normalize(mLookVec);
	
	XMVECTOR lookPoint = mPos + XMVector3Normalize(mLookVec);
	mRightVec = XMVector3Cross(mLookVec, mUpVec);
	mRightVec = XMVector3Normalize(mRightVec);

	mViewMatrix = XMMatrixLookAtLH(mPos, lookPoint, mUpVec);
}