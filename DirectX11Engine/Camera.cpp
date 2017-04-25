//Camera.cpp
#include "Camera.h"
#include <D3D11.h>

bool Camera::Init(XMVECTOR& storePos, XMVECTOR& storeLookAt, XMVECTOR& storeUp, float nearZ, float farZ, int width, int height, float fov)
{
	XMVECTOR storeRight;
	XMMATRIX view = XMMatrixLookAtLH(storePos, storeLookAt, storeUp);
	XMStoreFloat4x4(&mViewMatrix, view);

	storeLookAt = XMVector3Normalize(storeLookAt - storePos);
	storeUp = XMVector3Normalize(storeUp);
	storeRight = XMVector3Cross(storeLookAt, storeUp);
	storeRight = XMVector3Normalize(storeRight);

	storeUp = XMVector3Cross(storeRight, storeLookAt);
	storeUp = XMVector3Normalize(storeUp);

	XMStoreFloat3(&mLookVec, storeLookAt);
	XMStoreFloat3(&mUpVec, storeUp);
	XMStoreFloat3(&mRightVec, storeRight);
	XMStoreFloat3(&mPos, storePos);

	XMMATRIX proj = XMMatrixPerspectiveFovLH(fov, (float)width / (float)height, nearZ, farZ);
	XMStoreFloat4x4(&mProjectionMatrix, proj);
	return true;
}

void  Camera::MoveCameraHorizontally(float amount) 
{
	XMVECTOR storeRight = XMLoadFloat3(&mRightVec);
	XMVECTOR storeLookAt = XMLoadFloat3(&mLookVec);
	XMVECTOR storeUp = XMLoadFloat3(&mUpVec);

	XMVECTOR translationVec = -amount * storeRight;
	XMVECTOR movement = XMLoadFloat3(&mPos);

	movement += translationVec;
	XMStoreFloat3(&mPos, movement);
	XMMATRIX view = XMMatrixLookAtLH(movement, movement + storeLookAt, storeUp);
	XMStoreFloat4x4(&mViewMatrix, view);
}


void  Camera::MoveCameraForward(float amount)
{
	XMVECTOR storeRight = XMLoadFloat3(&mRightVec);
	XMVECTOR storeLookAt = XMLoadFloat3(&mLookVec);
	XMVECTOR storeUp = XMLoadFloat3(&mUpVec);

	XMVECTOR translationVec = -amount * storeLookAt;
	XMVECTOR movement = XMLoadFloat3(&mPos);

	movement += translationVec;
	XMStoreFloat3(&mPos, movement);
	XMMATRIX view = XMMatrixLookAtLH(movement, movement + storeLookAt, storeUp);
	XMStoreFloat4x4(&mViewMatrix, view);
}

void  Camera::MoveCameraVertically(float amount)
{
	XMVECTOR storeRight = XMLoadFloat3(&mRightVec);
	XMVECTOR storeLookAt = XMLoadFloat3(&mLookVec);
	XMVECTOR storeUp = XMLoadFloat3(&mUpVec);

	XMVECTOR translationVec = -amount * storeUp;
	XMVECTOR movement = XMLoadFloat3(&mPos);

	movement += translationVec;
	XMStoreFloat3(&mPos, movement);
	XMMATRIX view = XMMatrixLookAtLH(movement, movement + storeLookAt, storeUp);
	XMStoreFloat4x4(&mViewMatrix, view);
}


void Camera::ComboRotate(float amountX, float amountY)
{
	yRotation += XMConvertToRadians(-amountY);

	if (yRotation > 0.0f)
		yRotation = 0.0f;

	if (yRotation < -3.14129f / 2.0f)
		yRotation = -3.14129f / 2.0f;

	XMVECTOR storeRight = XMLoadFloat3(&mRightVec);
	XMVECTOR storeLookAt = XMLoadFloat3(&mLookVec);
	XMVECTOR storeUp = XMLoadFloat3(&mUpVec);
	XMVECTOR storePos = XMLoadFloat3(&mPos);

	XMMATRIX rotation = XMMatrixRotationY(amountX);
	storeLookAt = XMVector3Transform(storeLookAt, rotation);
	
	storeLookAt = XMVector3Normalize(storeLookAt);

	rotation = XMMatrixRotationAxis(storeRight, -amountY);
	storeLookAt = XMVector3Transform(storeLookAt, rotation);
	storeLookAt = XMVector3Normalize(storeLookAt);
	
	XMVECTOR lookPoint = storePos + XMVector3Normalize(storeLookAt);
	storeRight = XMVector3Cross(storeLookAt, storeUp);
	storeRight = XMVector3Normalize(storeRight);

	XMMATRIX view = XMMatrixLookAtLH(storePos, lookPoint, storeUp);
	XMStoreFloat4x4(&mViewMatrix, view);
	XMStoreFloat3(&mUpVec, storeUp);
	XMStoreFloat3(&mRightVec, storeRight);
	XMStoreFloat3(&mLookVec, storeLookAt);
}