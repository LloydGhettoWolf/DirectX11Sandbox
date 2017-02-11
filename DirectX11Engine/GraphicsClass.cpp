//GraphicsClass.cpp
#include "GraphicsClass.h"


bool GraphicsClass::Init(int screenWidth, int screenHeight, HWND hwnd)
{
	mD3D = new D3DClass();

	if (!mD3D) 
	{
		return false;
	}

	bool result = mD3D->Init(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, true, SCREEN_DEPTH, SCREEN_NEAR);

	if (!result) 
	{
		MessageBox(hwnd, L"couldn't init d3d obj", L"D3D11", MB_OK);
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

	return;
}


bool GraphicsClass::Frame()
{

	return Render();
}


bool GraphicsClass::Render()
{
	mD3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);
	mD3D->EndScene();

	return true;
}