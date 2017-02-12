//GraphicsClass.cpp
#include "GraphicsClass.h"


bool GraphicsClass::Init(int screenWidth, int screenHeight, HWND hwnd)
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
	mShader = new ColorShaderClass;
	if (!mShader)
	{
		return false;
	}

	// Initialize the color shader object.
	result = mShader->Initialize(mD3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
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


bool GraphicsClass::Frame()
{

	return Render();
}


bool GraphicsClass::Render()
{
	XMMATRIX viewMatrix, projMatrix, worldMatrix;
	bool result;


	// Clear the buffers to begin the scene.
	mD3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);


	// Get the world, view, and projection matrices from the camera and d3d objects.
	worldMatrix = *(mD3D->GetWorldMatrix());
	projMatrix = *(mD3D->GetProjectionMatrix());

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	mMesh->Render(mD3D->GetDeviceContext());

	// Render the model using the color shader.
	result = mShader->Render(mD3D->GetDeviceContext(), mMesh->GetIndexCount(), worldMatrix, viewMatrix, projMatrix);
	if (!result)
	{
		return false;
	}
	mD3D->EndScene();

	return true;
}