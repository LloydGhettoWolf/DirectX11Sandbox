//TestApp.cpp//TestApp.cpp
#include <d3d11.h>
#include <D3DClass.h>
#include <Frustum.h>
#include <OutlineShader.h>
#include <ObjParser.h>
#include <Texture.h>
#include <dinput.h>
#include <Camera.h>
#include <Utility.h>
#include <vector>
#include <SimpleMesh.h>
#include "TestApp.h"
#include "DefaultDiffuseShader.h"
#include "SpecMapShader.h"
#include "NormalMapShader.h"
#include "NormNoSpecMapShader.h"

#define DIRECTINPUT_VERSION 0x0800

using namespace std;

bool TestApp::Init(int screenWidth, int screenHeight, HWND hwnd, HINSTANCE hInstance)
{
	mD3D = new D3DClass();

	if (!mD3D)
	{
		return false;
	}

	mScreenHeight = 600;
	mScreenWidth = 800;

	HRESULT result = mD3D->Init(mScreenWidth, mScreenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);

	if (!result)
	{
		MessageBox(hwnd, "couldn't init d3d obj", "D3D11", MB_OK);
		return false;
	}


	// Initialize the model object.

	wstring path(L"C://Users/GhettoFett/Documents/processedMeshes/");
	wstring name(L"sponza.boom");
	string strName(name.begin(), name.end());
	string strPath(path.begin(), path.end());



	ProcessedMeshData* meshData;
	string* textureNames;
	ProcessedNormalMappedMeshData* mappedMeshData;
	ReadBoomFile(strPath, strName, mNumMeshes, mNumMappedMeshes, mNumMaterials, mNumTextures, &textureNames, &meshData, &mMaterialProperties);

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
			MessageBox(hwnd, "Could not initialize the model object.", "Error", MB_OK);
			return false;
		}
	}

	delete[] meshData;

	

	//init textures
	mTextures = new Texture[mNumTextures];
	for (unsigned int i = 0; i < mNumTextures; i++)
	{

		wstring wstr(textureNames[i].begin(), textureNames[i].end());
		result = mTextures[i].InitFromDDS(mD3D->GetDevice(), &wstr[0]);
		if (!result)
		{
			MessageBox(hwnd, "Could not initialize the model object.", "Error", MB_OK);
			return false;
		}
	}

	delete[] textureNames;

	// Create the color shader object.
	mMeshShaders[DIFF_SHADER] = new DefaultDiffuseShader();
	if (!mMeshShaders[DIFF_SHADER])
	{
		return false;
	}


	// Initialize the color shader object.
	result = mMeshShaders[DIFF_SHADER]->Init(mD3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the color shader object.", "Error", MB_OK);
		return false;
	}

	mOutlineShader = new OutlineShader();

	result = mOutlineShader->Init(mD3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the outline shader object.", "Error", MB_OK);
		return false;
	}

	mMeshShaders[SPEC_SHADER] = new SpecMapShader();
	if (!mMeshShaders[SPEC_SHADER])
	{
		return false;
	}

	result = mMeshShaders[SPEC_SHADER]->Init(mD3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the spec shader object.", "Error", MB_OK);
		return false;
	}

	mMeshShaders[NORMMAP_SHADER] = new NormalMapShader();
	if (!mMeshShaders[NORMMAP_SHADER])
	{
		return false;
	}

	result = mMeshShaders[NORMMAP_SHADER]->Init(mD3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the norm shader object.", "Error", MB_OK);
		return false;
	}

	mMeshShaders[NORM_NO_SPEC_SHADER] = new NormNoSpecMapShader();
	if (!mMeshShaders[NORM_NO_SPEC_SHADER])
	{
		return false;
	}

	result = mMeshShaders[NORM_NO_SPEC_SHADER]->Init(mD3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the norm no spec shader object.", "Error", MB_OK);
		return false;
	}

	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR position = XMVectorSet(0.0f, 50.0f, 0.0f, 0.0f);
	XMVECTOR lookAt = XMVectorSet(10.0f, 50.0f, 0.0f, 0.0f);

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

	mFrustum = new Frustum();

	return true;
}


void TestApp::Shutdown()
{
	if (mD3D)
	{
		mD3D->Shutdown();
		delete mD3D;
	}

	// Release the color shader object.
	if (mMeshShaders[0])
	{
		delete mMeshShaders[0];
	}

	if (mMeshShaders[1])
	{
		delete mMeshShaders[1];
	}

	if (mMeshShaders[2])
	{
		delete mMeshShaders[2];
	}

	if (mMeshShaders[3])
	{
		delete mMeshShaders[3];
	}

	if (mOutlineShader)
	{
		mOutlineShader->Shutdown();
		delete mOutlineShader;
	}

	// Release the model object.
	if (mMesh)
	{
		for (unsigned int i = 0; i < mNumMeshes; i++)
		{
			mMesh[i].Shutdown();
		}

		delete[] mMesh;
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
		for (unsigned int i = 0; i < mNumTextures; i++)
			mTextures[i].Shutdown();
	}

	if (mMaterialProperties)
	{
		delete[] mMaterialProperties;
	}

	return;
}


bool TestApp::Frame(DIMOUSESTATE& state)
{
	ReadInput(state);

	//update camera
	mCamera->ComboRotate((float)mMouseRotateX, (float)mMouseRotateY);
	mCamera->MoveCameraVertically((float)mMouseVertY);
	mCamera->MoveCameraForward((float)mMouseHorizZ);

	mFrustum->UpdateFrustum(mCamera);

	return Render();
}

bool TestApp::ReadInput(DIMOUSESTATE& state)
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


bool TestApp::Render()
{
	XMMATRIX viewMatrix, projMatrix, worldMatrix;
	bool result;

	ID3D11DeviceContext* context = mD3D->GetDeviceContext();

	MatrixBufferType matrices;

	matrices.world = XMMatrixTranspose(mWorld);
	matrices.projection = XMMatrixTranspose(mCamera->GetProjection());
	matrices.view = XMMatrixTranspose(mCamera->GetView());

	LightPosBuffer lights;
	XMStoreFloat3(&lights.eyePos, mCamera->GetPos());
	static float theta = 0.0f;
	float xPos = sin(theta) * 700.0f;

	lights.lightPos = XMFLOAT3(xPos, 50.0f, 0.0f);
	lights.lightCol = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);

	theta += 0.05f;

	// Clear the buffers to begin the scene.
	mD3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);


	vector<SimpleMesh*> unculledMeshes = CullMeshesAgainstFrustum(mMesh, mNumMeshes, mFrustum);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	unsigned int numUnculledMeshes = (unsigned int)unculledMeshes.size();
	Shader* thisShader = nullptr;
	ID3D11ShaderResourceView* diffSrv = nullptr;
	ID3D11ShaderResourceView* specSrv = nullptr;
	ID3D11ShaderResourceView* normSrv = nullptr;
	ID3D11ShaderResourceView* maskSrv = nullptr;

	for (unsigned int i = 0; i < numUnculledMeshes; i++)
	{
		// Render the model using the color shader.
		int materialIndex = unculledMeshes[i]->GetMaterialIndex();
		materialInfo material = mMaterialProperties[materialIndex];
		if (material.maskIndex != 0)
		{
			diffSrv = mTextures[material.maskIndex].GetTexture();
			maskSrv = mTextures[material.maskIndex].GetTexture();
			normSrv = mTextures[material.normMapIndex].GetTexture();
			continue;
		}
		else if (material.specTexIndex != 0 && material.normMapIndex != 0)
		{
			normSrv = mTextures[material.normMapIndex].GetTexture();
			specSrv = mTextures[material.specTexIndex].GetTexture();
			thisShader = mMeshShaders[NORMMAP_SHADER];
		}
		else if (material.normMapIndex != 0)
		{
			normSrv = mTextures[material.normMapIndex].GetTexture();
			thisShader = mMeshShaders[NORM_NO_SPEC_SHADER];
		}
		else if (material.specTexIndex != 0)
		{
			specSrv = mTextures[material.specTexIndex].GetTexture();
			thisShader = mMeshShaders[SPEC_SHADER];
		}
		else
		{
			thisShader = mMeshShaders[DIFF_SHADER];
		}
			
		result = thisShader->PrepareShader(context);

		ConstantsStruct constants;
		constants.lightPtr = &lights;
		constants.matPtr = &matrices;
		result = thisShader->SetConstantShaderParameters((void*)&constants, context);

		MaterialProperties materialProperties;
		materialProperties.diffuseCol = XMFLOAT4(material.diffuse.x, material.diffuse.y, material.diffuse.z, 1.0f);
		materialProperties.specComponent = material.specFactor;

		diffSrv = mTextures[material.diffTexIndex].GetTexture();

		NormShaderPerMeshStruct perMesh;
		perMesh.material = &materialProperties;
		perMesh.sampler = mSamplerState;
		perMesh.diffuseSrv = diffSrv;
		perMesh.specSrv = specSrv;
		perMesh.normSrv = normSrv;

		thisShader->SetPerMeshParameters(static_cast<void*>(&perMesh), mD3D->GetDeviceContext());
		
		unculledMeshes[i]->Render(context);
	}


#if RENDER_AABB
	result = mOutlineShader->PrepareShader(context);

	if (!result)
	{
		return false;
	}

	result = mOutlineShader->SetConstantShaderParameters(static_cast<void*>(&matrices), context);

	if (!result)
	{
		return false;
	}

	for (unsigned int i = 0; i < mNumMeshes; i++)
	{
		mMesh[i].RenderBoundingBox(context);
	}
#endif

	mD3D->EndScene();

	return true;
}