#include "ComputeApp.h"
#include <ComputeShader.h>
#include <D3DClass.h>
#include <d3d11.h>

bool ComputeApp::Init(int screenWidth, int screenHeight, HWND hwnd, HINSTANCE hInstance)
{
	mD3D = new D3DClass();

	if (!mD3D)
	{
		return false;
	}

	mScreenHeight = static_cast<int>(600);
	mScreenWidth = static_cast<int>(800);

	HRESULT result = mD3D->Init(mScreenWidth, mScreenHeight, true, hwnd, false, 300.0f, 0.1f, 1, 1);

	if (!result)
	{
		MessageBox(hwnd, L"couldn't init d3d obj", L"D3D11", MB_OK);
		return false;
	}

	float data[100];

	for (unsigned int i = 0; i < 100; i++)
	{
		data[i] = 0.2f * (float)i;
	}

	mCompShader = new ComputeShader();

	result = mCompShader->Init(mD3D->GetDevice(), 
						hwnd,
						L"C://Users/GhettoFett/Documents/Visual Studio 2015/Projects/DirectX11Engine/Debug/ComputeShader.cso", 
						data, 
						100);

	if (!result)
	{
		MessageBox(hwnd, L"couldn't init comp shader", L"D3D11", MB_OK);
		return false;
	}

	mCompShader->PrepareShader(mD3D->GetDeviceContext());
	mD3D->GetDeviceContext()->Dispatch(100, 1, 1);
	mCompShader->CheckContents(mD3D->GetDeviceContext());
	return true;
}

void ComputeApp::Shutdown()
{
	if (mCompShader)
		delete mCompShader;
}

bool ComputeApp::Frame(DIMOUSESTATE& state)
{
	return true;
}