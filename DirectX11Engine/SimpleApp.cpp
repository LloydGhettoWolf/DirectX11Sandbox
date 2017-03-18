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


	// Initialize the model object.

	wstring path(L"C://Users/GhettoFett/Documents/processedMeshes/");
	wstring name(L"sponza.boom");
	string strName(name.begin(), name.end());
	string strPath(path.begin(), path.end());

	

	ProcessedMeshData* meshData;
	string* textureNames;
	ReadBoomFile(strPath, strName, mNumMeshes, mNumMaterials, mNumTextures, &textureNames, &meshData, &mMaterialProperties);

	// Create the model object.
	mMesh = new SimpleMesh[mNumMeshes];
	if (!mMesh)
	{
		return false;
	}

	for (unsigned int i = 0; i < mNumMeshes; i++)
	{
		result = mMesh[i].Init(mD3D->GetDevice(), &meshData[i]);

		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}
	}

	delete[] meshData;

	//init textures
	mTextures = new Texture[mNumTextures];
	for (unsigned int i = 0; i < mNumTextures; i++)
	{
		wstring wstr(textureNames[i].begin(), textureNames[i].end());
		result = mTextures[i].Init(mD3D->GetDevice(), &wstr[0]);

		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the textures", L"Error", MB_OK);
			return false;
		}
	}

	delete[] textureNames;

	// Create the color shader object.
	mShader = new DefaultDiffuseShader;
	if (!mShader)
	{
		return false;
	}

	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];

	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "NORMAL";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "TEXCOORD";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	// Initialize the color shader object.
	result = mShader->Init(mD3D->GetDevice(), hwnd, L"SimpleVertexShader.hlsl", L"SimplePixelShader.hlsl", polygonLayout, 3);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}

	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR position = XMVectorSet(0.0f, 150.0f, 0.0f, 0.0f);
	XMVECTOR lookAt = XMVectorSet(0.0f, 150.0f, 10.0f, 0.0f);
	
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
	mMouseRotateX = 0;
	mMouseRotateY = 0;
	mMouseVertY = 0;

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
		for (int i = 0; i < mNumMeshes; i++)
		{
			mMesh[i].Shutdown();
		}

		delete [] mMesh;
	}

	if (mSamplerState)
	{
		mSamplerState->Release();
	}

	if (mCamera)
	{
		delete mCamera;
	}

	if (mTextures)
	{
		for (int i = 0; i < mNumTextures; i++)
			mTextures[i].Shutdown();
	}

	if (mMaterialProperties)
	{
		delete  [] mMaterialProperties;
	}

	return;
}


bool SimpleApp::Frame(DIMOUSESTATE& state)
{
	
	ReadInput(state);

	//update camera
	mCamera->ComboRotate((float)mMouseRotateX, (float)mMouseRotateY);
	mCamera->MoveCameraVertically((float)mMouseVertY);
	mCamera->MoveCameraForward((float)mMouseHorizZ);
	
	return Render();
}

bool SimpleApp::ReadInput(DIMOUSESTATE& state)
{
	//read control
	if (state.rgbButtons[0] & 0x80)
	{
		mMouseRotateX = state.lX;
		mMouseRotateY = state.lY;
	}

	if (state.rgbButtons[1] & 0x80)
	{
		mMouseHorizZ = state.lY;
	}

	if (state.rgbButtons[2] & 0x80)
	{
		mMouseVertY = state.lY;
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
	XMStoreFloat3(&lights.lightPos, mCamera->GetPos());
	lights.lightCol = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	
	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	for (unsigned int i = 0; i < mNumMeshes; i++)
	{
		// Render the model using the color shader.
		int materialIndex = mMesh[i].GetMaterialIndex();

		materialInfo material = mMaterialProperties[materialIndex];
		MaterialProperties materialProperties;
		materialProperties.diffuseCol = XMFLOAT4(material.diffuse.x, material.diffuse.y, material.diffuse.z, 1.0f);
		materialProperties.specCol = XMFLOAT4(material.specular.x, material.specular.y, material.specular.z, 1.0f);
		materialProperties.specComponent = material.specFactor;

		ID3D11ShaderResourceView* srv = mTextures[material.diffTexIndex].GetTexture();

	    result = mShader->PrepareShader(context, &matrices, &lights, &materialProperties, srv, mSamplerState);

		if (!result)
		{
			return false;
		}

		mMesh[i].Render(context);
	}

	mD3D->EndScene();

	return true;
}