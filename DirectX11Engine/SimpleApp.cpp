//SimpleApp.cpp
#include <Windows.h>
#include "SimpleApp.h"
#include "D3DClass.h"
#include "SimpleMesh.h"
#include "Camera.h"

#define DIRECTINPUT_VERSION 0x0800


bool SimpleApp::Init(int screenWidth, int screenHeight, HWND hwnd, HINSTANCE hInstance)
{
	mD3D = new D3DClass();

	if (!mD3D) 
	{
		return false;
	}

	mScreenHeight = screenHeight;
	mScreenWidth = screenWidth;

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
	result = mMesh->Init(mD3D->GetDevice(), L"C://Users/GhettoFett/Documents/processedMeshes/buddha.boom");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the color shader object.
	mShader = new OneLightShader;
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

	polygonLayout[1].SemanticName = "NORMAL";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	/*polygonLayout[2].SemanticName = "TEXCOORD";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;*/

	// Initialize the color shader object.
	result = mShader->Init(mD3D->GetDevice(), hwnd, L"SimpleVertexShader.hlsl", L"SimplePixelShader.hlsl", polygonLayout, 2);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}

	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR position = XMVectorSet(0.0f, 10.0f, -25.0f, 0.0f);
	XMVECTOR lookAt = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	
	float fieldOfView = (float)XM_PI / 4.0f;

	mCamera = new Camera();
	mCamera->Init(position, lookAt, up, SCREEN_NEAR, SCREEN_DEPTH, screenWidth, screenHeight, fieldOfView);

	// Initialize the world matrix to the identity matrix.
	mWorld = XMMatrixIdentity();

	D3D11_SAMPLER_DESC samplerDesc;

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	result = mD3D->GetDevice()->CreateSamplerState(&samplerDesc, &mSamplerState);
	if (FAILED(result))
	{
		return false;
	}

	//input creation

	mMouseX = 0;
	mMouseY = 0;

	result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dInput, NULL);
	if (FAILED(result))
	{
		return false;
	}

	result = dInput->CreateDevice(GUID_SysMouse, &mMouse, NULL);
	if (FAILED(result))
	{
		return false;
	}

	result = mMouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		return false;
	}

	result = mMouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	result = mMouse->Acquire();
	if (FAILED(result))
	{
		return false;
	}

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

	if (mSamplerState)
	{
		mSamplerState->Release();
	}

	if (mCamera)
	{
		delete mCamera;
	}

	return;
}


bool SimpleApp::Frame()
{
	//rotate buddha
	static float y = 0.0f;
	mWorld = XMMatrixRotationY(y);
	y += 0.01f;

	
	ReadInput();
	mCamera->ComboRotate((float)mMouseX, (float)mMouseY);


	//update camera

	return Render();
}

bool SimpleApp::ReadInput()
{
	//read controls
	HRESULT result;

	// Read the mouse device.
	result = mMouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mMouseState);

	if (FAILED(result))
	{
		// If the mouse lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			mMouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	if (mMouseState.rgbButtons[0] & 0x80)
	{
		mMouseX = mMouseState.lX;
		mMouseY = mMouseState.lY;
	}

	return true;
}


bool SimpleApp::Render()
{
	XMMATRIX viewMatrix, projMatrix, worldMatrix;
	bool result;

	// Clear the buffers to begin the scene.
	mD3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	ID3D11DeviceContext* context = mD3D->GetDeviceContext();

	MatrixBufferType matrices;

	matrices.world = XMMatrixTranspose(mWorld);
	matrices.projection = XMMatrixTranspose( mCamera->GetProjection());
	matrices.view = XMMatrixTranspose( mCamera->GetView());

	LightPosBuffer lights;
	lights.lightPos = XMFLOAT3(0.0f, 10.0f, -25.0f);
	lights.lightCol = XMFLOAT3(1.0f, 1.0f, 1.0f);
	
	// Render the model using the color shader.
	result = mShader->PrepareShader(context, &matrices, &lights, 0, mSamplerState);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	mMesh->Render(context);
	

	if (!result)
	{
		return false;
	}

	mD3D->EndScene();

	return true;
}