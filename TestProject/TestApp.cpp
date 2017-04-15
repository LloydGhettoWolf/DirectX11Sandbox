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
#include "AlphaMaskShader.h"
#include "BasicPointSpriteShader.h"
#include <Renderer.h>
#include <ResourceAllocator.h>
#include <PointSprite.h>
#include <algorithm>

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

	mResourceAllocator = new ResourceAllocator(mD3D->GetDevice());
	mRenderer = new Renderer(mD3D->GetDevice(), mD3D->GetDeviceContext());

	// Initialize the model object.

	wstring path(L"C://Users/GhettoFett/Documents/processedMeshes/");
	wstring name(L"sponza.boom");
	string strName(name.begin(), name.end());
	string strPath(path.begin(), path.end());

	ProcessedMeshData* meshData;
	string* textureNames;
	ProcessedNormalMappedMeshData* mappedMeshData;
	materialInfo* materialProperties;
	ReadBoomFile(strPath, strName, mNumMeshes, mNumMappedMeshes, mNumMaterials, mNumTextures, &textureNames, &meshData, &materialProperties);

	// Create the model object.
	mMesh = new SimpleMesh[mNumMeshes];
	if (!mMesh)
	{
		return false;
	}

	for (unsigned int i = 0; i < mNumMeshes; i++)
	{
		result = mMesh[i].Init(mResourceAllocator, &meshData[i]);

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

	mMeshShaders[MASK_SHADER] = new AlphaMaskShader();
	if (!mMeshShaders[MASK_SHADER])
	{
		return false;
	}

	result = mMeshShaders[MASK_SHADER]->Init(mD3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the norm no spec shader object.", "Error", MB_OK);
		return false;
	}

	mMeshShaders[INSTANCE_SHADER] = new PSShader();
	if (!mMeshShaders[INSTANCE_SHADER])
	{
		return false;
	}

	result = mMeshShaders[INSTANCE_SHADER]->Init(mD3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the point sprite shader object.", "Error", MB_OK);
		return false;
	}


	//now meshes, textures and materials are created, we really should try and pair materials with shaders
	PairMaterialsWithShaders(materialProperties);
	delete[] materialProperties;

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

	//create blend state
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));

	blendDesc.RenderTarget[0].BlendEnable			= true;
	blendDesc.RenderTarget[0].SrcBlend				= D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend				= D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp				= D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha			= D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha		= D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha          = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	result = mD3D->GetDevice()->CreateBlendState(&blendDesc, &mBlendState);
	if (FAILED(result))
	{
		return false;
	}

	//input creation

	mMouseRotateX = 0;
	mMouseRotateY = 0;
	mMouseVertY = 0;

	mFrustum = new Frustum();

	//create point sprite
	XMFLOAT3 pos[4] = { XMFLOAT3(0.0f, 0.0f, 0.0f), 
						XMFLOAT3(0.0f, 0.0f, 0.0f), 
						XMFLOAT3(0.0f, 0.0f, 0.0f), 
						XMFLOAT3(0.0f, 0.0f, 0.0f) };
	mPointSprite = new PointSprite(1, &pos[0]);

	if (!mPointSprite->Init(mResourceAllocator))
	{
		return false;
	}

	return true;
}

bool TestApp::PairMaterialsWithShaders(materialInfo* materials)
{
	mMaterials = new processedMaterialInfo[mNumMaterials];

	for (unsigned int i = 0; i < mNumMaterials; i++)
	{
		materialInfo* info = &materials[i];

		mMaterials[i].specular = materials[i].specular;
		mMaterials[i].diffuse  = materials[i].diffuse;
		
		mMaterials[i].diffSrv = mTextures[info->diffTexIndex].GetTexture();

		if (info->maskIndex != 0)
		{
			mMaterials[i].maskSrv = mTextures[info->maskIndex].GetTexture();
			mMaterials[i].shader = mMeshShaders[MASK_SHADER];
		}
		else if (info->specTexIndex != 0 && info->normMapIndex != 0)
		{
			mMaterials[i].normSrv = mTextures[info->normMapIndex].GetTexture();
			mMaterials[i].specSrv = mTextures[info->specTexIndex].GetTexture();
			mMaterials[i].shader = mMeshShaders[NORMMAP_SHADER];
		}
		else if (info->normMapIndex != 0)
		{
			mMaterials[i].normSrv = mTextures[info->normMapIndex].GetTexture();
			mMaterials[i].shader = mMeshShaders[NORM_NO_SPEC_SHADER];
		}
		else if (info->specTexIndex != 0)
		{
			mMaterials[i].specSrv = mTextures[info->specTexIndex].GetTexture();
			mMaterials[i].shader = mMeshShaders[SPEC_SHADER];
		}
		else
		{
			mMaterials[i].shader = mMeshShaders[DIFF_SHADER];
		}
		
	}
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

	if (mMeshShaders[4])
	{
		delete mMeshShaders[4];
	}

	/*if (mOutlineShader)
	{
		mOutlineShader->Shutdown();
		delete mOutlineShader;
	}*/

	// Release the model object.
	if (mMesh)
	{
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

	if (mMaterials)
	{
		delete[] mMaterials;
	}

	if (mPointSprite)
	{
		delete mPointSprite;
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
	float xPos = sin(theta) * 1100.0f;

	lights.lightPos = XMFLOAT3(xPos, 150.0f, 0.0f);
	lights.lightCol = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);

	theta += 0.01f;

	// Clear the buffers to begin the scene.
	mD3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);


	vector<SimpleMesh*> unculledMeshes = CullMeshesAgainstFrustum(mMesh, mNumMeshes, mFrustum);
	XMMATRIX projView = mCamera->GetProjection() * mCamera->GetView();
	unsigned int numUnculledMeshes = (unsigned int)unculledMeshes.size();

	for (unsigned int i = 0; i < numUnculledMeshes; i++)
	{
		UpdateThisFrameZ(unculledMeshes[i], &projView);
	}
	std::sort(unculledMeshes.begin(), unculledMeshes.end(), CompareZAscend);

	vector<SimpleMesh*> alphaMeshes;

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.

	Shader* thisShader = nullptr;
	ID3D11ShaderResourceView* diffSrv = nullptr;
	ID3D11ShaderResourceView* specSrv = nullptr;
	ID3D11ShaderResourceView* normSrv = nullptr;
	ID3D11ShaderResourceView* alphaSrv = nullptr;

	for (unsigned int i = 0; i < unculledMeshes.size();)
	{ 
		int materialIndex = unculledMeshes[i]->GetMaterialIndex();
		processedMaterialInfo* info = &mMaterials[materialIndex];

		if (info->maskSrv != 0)
		{
			alphaMeshes.emplace_back(unculledMeshes[i]);
			unculledMeshes.erase(unculledMeshes.begin() + i);
			continue;
		}

		i++;
	}

	RenderMeshList(unculledMeshes, &lights, &matrices);

	unsigned int numAlphaMeshes = (unsigned int)alphaMeshes.size();
	for (unsigned int i = 0; i < numAlphaMeshes; i++)
	{
		UpdateThisFrameZ(alphaMeshes[i], &projView);
	}
	std::sort(alphaMeshes.begin(), alphaMeshes.end(), CompareZDescend);

	context->OMSetBlendState(mBlendState, 0, 0xffffffff);
	
	//RenderMeshList(unculledMeshes, &lights, &matrices);

	context->OMSetBlendState(NULL, 0, 0xffffffff);

	//set shader for point sprite
	mMeshShaders[INSTANCE_SHADER]->PrepareShader(mD3D->GetDeviceContext());
	ConstantsStruct constants;
	constants.lightPtr = &lights;
	constants.matPtr = &matrices;
	result = mMeshShaders[INSTANCE_SHADER]->SetConstantShaderParameters((void*)&constants, context);

	ID3D11Buffer* pointVerts = mPointSprite->GetVertBuffer();
	ID3D11Buffer* instanceData = mPointSprite->GetInstanceBuffer();
	ID3D11Buffer* vertBuffers[2] = { pointVerts, instanceData };
	unsigned int strides[2] = { sizeof(XMFLOAT3) , sizeof(XMFLOAT3) };
	mRenderer->DrawIndexedInstanced(&vertBuffers[0], 2, strides, mPointSprite->GetIndexBuffer(), 6, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 1);

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

void TestApp::RenderMeshList(vector<SimpleMesh*>& meshes, LightPosBuffer* lights, MatrixBufferType* matrices)
{
	unsigned int numMeshesInList = meshes.size();
	ID3D11DeviceContext* context = mD3D->GetDeviceContext();

	for (unsigned int i = 0; i < numMeshesInList; i++)
	{
		// Render the model using the color shader.
		int materialIndex = meshes[i]->GetMaterialIndex();
		processedMaterialInfo* info = &mMaterials[materialIndex];

		HRESULT result = info->shader->PrepareShader(context);

		ConstantsStruct constants;
		constants.lightPtr = lights;
		constants.matPtr = matrices;
		result = info->shader->SetConstantShaderParameters((void*)&constants, context);

		MaterialProperties materialProperties;
		materialProperties.diffuseCol = XMFLOAT4(info->diffuse.x, info->diffuse.y, info->diffuse.z, 1.0f);
		materialProperties.specCol = XMFLOAT4(info->specular.x, info->specular.y, info->specular.z, 1.0f);
		materialProperties.specComponent = 10.0f;

		ShaderPerMeshStruct perMesh;
		perMesh.material = &materialProperties;
		perMesh.sampler = mSamplerState;
		perMesh.diffuseSrv = info->diffSrv;
		perMesh.specSrv = info->specSrv;
		perMesh.maskSrv = info->maskSrv;
		perMesh.normSrv = info->normSrv;

		info->shader->SetPerMeshParameters(static_cast<void*>(&perMesh), mD3D->GetDeviceContext());

		mRenderer->DrawIndexed(meshes[i]->GetVertBuffer(),
			meshes[i]->GetNumBuffers(),
			meshes[i]->GetStrides(),
			meshes[i]->GetIndexBuffer(),
			meshes[i]->GetIndexCount(),
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
}