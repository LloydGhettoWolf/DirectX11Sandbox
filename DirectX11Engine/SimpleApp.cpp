//SimpleApp.cpp
#include <Windows.h>
#include "SimpleApp.h"
#include "D3DClass.h"
#include "SimpleMesh.h"


bool SimpleApp::Init(int screenWidth, int screenHeight, HWND hwnd)
{
	mD3D = new D3DClass();

	if (!mD3D) 
	{
		return false;
	}

	bool result = mD3D->Init(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);

	if (!result) 
	{
		MessageBox(hwnd, L"couldn't init d3d obj", L"D3D11", MB_OK);
		return false;
	}

	// Create the model object.
	mMesh = new SimpleMesh();
	if (!mMesh)
	{
		return false;
	}

	// Initialize the model object.
	result = mMesh->Init(mD3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the color shader object.
	mShader = new ShaderClass;
	if (!mShader)
	{
		return false;
	}

	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];

	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	// Initialize the color shader object.
	result = mShader->Init(mD3D->GetDevice(), hwnd, L"SimpleVertexShader.hlsl", L"SimplePixelShader.hlsl", polygonLayout, 2);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}

	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR position = XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f);
	XMVECTOR lookAt = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	mView = XMMatrixTranspose(XMMatrixLookAtLH(position, lookAt, up));

	float fieldOfView = (float)XM_PI / 4.0f;
	float screenAspect = (float)screenWidth / (float)screenHeight;

	mProj = XMMatrixTranspose(XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, SCREEN_NEAR, SCREEN_DEPTH));

	// Initialize the world matrix to the identity matrix.
	mWorld = XMMatrixTranspose(XMMatrixIdentity());

	return true;
}


void SimpleApp::Shutdown()
{
	if (mD3D)
	{
		mD3D->Shutdown();
		delete mD3D;
	}

	// Release the color shader object.
	if (mShader)
	{
		mShader->Shutdown();
		delete mShader;
	}

	// Release the model object.
	if (mMesh)
	{
		mMesh->Shutdown();
		delete mMesh;
	}

	return;
}


bool SimpleApp::Frame()
{

	return Render();
}


bool SimpleApp::Render()
{
	XMMATRIX viewMatrix, projMatrix, worldMatrix;
	bool result;


	// Clear the buffers to begin the scene.
	mD3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	ID3D11DeviceContext* context = mD3D->GetDeviceContext();

	MatrixBufferType matrices;
	matrices.world = mWorld;
	matrices.projection = mProj;
	matrices.view = mView;
	
	// Render the model using the color shader.
	result = mShader->PrepareShader(context, &matrices);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	mMesh->Render(context);
	

	if (!result)
	{
		return false;
	}

	mD3D->EndScene();

	return true;
}