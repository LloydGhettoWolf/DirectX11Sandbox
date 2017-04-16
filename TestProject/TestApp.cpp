//TestApp.cpp//TestApp.cpp
#include <memory>
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
#include <Renderer.h>
#include <ResourceAllocator.h>
#include <PointSprite.h>
#include <algorithm>
#include "TestApp.h"
#include "DefaultDiffuseShader.h"
#include "SpecMapShader.h"
#include "NormalMapShader.h"
#include "NormNoSpecMapShader.h"
#include "AlphaMaskShader.h"
#include "BasicPointSpriteShader.h"



#define DIRECTINPUT_VERSION 0x0800

using namespace std;

ShaderCreator createShaders[] =
{
	CreateShaderType<DefaultDiffuseShader>,
	CreateShaderType<SpecMapShader>,
	CreateShaderType<NormalMapShader>,
	CreateShaderType<NormNoSpecMapShader>,
	CreateShaderType<AlphaMaskShader>,
	CreateShaderType<PSShader>
};

bool TestApp::Init(int screenWidth, int screenHeight, HWND hwnd, HINSTANCE hInstance)
{
	mD3D = new D3DClass();

	if (!mD3D)
	{
		return false;
	}

	mScreenHeight = SCREEN_HEIGHT;
	mScreenWidth = SCREEN_WIDTH;

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

	delete [] meshData;

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

	////////////////////////////////////////////////////////////
	//Create Shaders
	for (unsigned int i = DIFF_SHADER; i <= INSTANCE_SHADER; i++)
	{
		// Create the color shader object.
		mMeshShaders[i] = createShaders[i]();
		if (!mMeshShaders[i])
		{
			return false;
		}

		// Initialize the color shader object.
		result = mMeshShaders[i]->Init(mD3D->GetDevice(), hwnd);
		if (!result)
		{
			MessageBox(hwnd, "Could not initialize the color shader object.", "Error", MB_OK);
			return false;
		}
	}


	mOutlineShader = new OutlineShader();

	////////////////////////////////////////////////////////////////////
	//now meshes, textures and materials are created, we really should try and pair materials with shaders
	PairMaterialsWithShaders(materialProperties);
	delete[] materialProperties;


	/////////////////////////////////////////////////////////////////////
	//Create camera and frustum info
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR position = XMVectorSet(0.0f, 10.0f, 0.0f, 0.0f);
	XMVECTOR lookAt = XMVectorSet(10.0f, 10.0f, 0.0f, 0.0f);

	float fieldOfView = (float)XM_PI / 4.0f;

	mCamera = new Camera();
	mCamera->Init(position, lookAt, up, SCREEN_NEAR, SCREEN_DEPTH, screenWidth, screenHeight, fieldOfView);
	mFrustum = new Frustum();

	// Initialize the world matrix to the identity matrix.
	mWorld = XMMatrixScaling(0.05f, 0.05f, 0.05f);

	//update all the bounding boxes for the meshes according to the matrix
	UpdateBoundingBoxes(mMesh, mNumMeshes, &mWorld);

	///////////////////////////////////////////////////////////////
	//Descriptors for samples, blends etc
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

	//create point sprite
	XMFLOAT3 pos[1] = { XMFLOAT3(100.0f, 100.0f, 0.0f)};
	XMFLOAT3 col[1] = { XMFLOAT3(1.0f, 0.0f, 0.0f) };
	mPointSprite = new PointSprite(1, &pos[0], &col[0]);

	if (!mPointSprite->Init(mResourceAllocator, mD3D->GetDevice(), L"pointSprite.dds"))
	{
		MessageBox(hwnd, "Could not initialize the point sprite.", "Error", MB_OK);
		return false;
	}
	
	//get initial time for timer
	mLastTime = chrono::high_resolution_clock::now();

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
	if (mMeshShaders)
		delete [] mMeshShaders;

	/*if (mOutlineShader)
	{
		mOutlineShader->Shutdown();
		delete mOutlineShader;
	}*/

	// Release the model object.
	if (mMesh)
		delete[] mMesh;

	if (mSamplerState)
		mSamplerState->Release();

	if (mBlendState)
		mBlendState->Release();


	if (mCamera)
		delete mCamera;

	if (mTextures)
		delete[] mTextures;

	if (mMaterials)
		delete[] mMaterials;

	if (mPointSprite)
		delete mPointSprite;

	return;
}


bool TestApp::Frame(DIMOUSESTATE& state)
{
	chrono::high_resolution_clock::time_point currentTime = chrono::high_resolution_clock::now();
	chrono::duration<double, std::milli> span = currentTime - mLastTime;
	mLastTime = currentTime;
	double frameTime = span.count();
	frameTime *= 0.001; //count() is in millis, so divide by 1000 to get time in secs

	while (frameTime > 0.0)
	{
		float delta = min(frameTime, DELTA_TIME);
		ReadInput(state);

		//update camera
		mCamera->ComboRotate((float)mMouseRotateX * delta * 0.1f, (float)mMouseRotateY * delta * 0.1f);
		mCamera->MoveCameraVertically((float)mMouseVertY * delta * VERT_MOVEMENT_SPEED);
		mCamera->MoveCameraForward((float)mMouseHorizZ * delta * FORWARD_MOVEMENT_SPEED);
		frameTime -= delta;
	}

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
	else
	{
		mMouseRotateX = 0;
		mMouseRotateY = 0;
	}

	if (state.rgbButtons[1] & 0x80)
		mMouseHorizZ = state.lY;
	else
		mMouseHorizZ = 0;

	if (state.rgbButtons[2] & 0x80)
		mMouseVertY = state.lY;
	else
		mMouseVertY = 0;

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
	float xPos = sin(theta) * 60.0f;

	lights.lightPos = XMFLOAT3(xPos, 10.0f, 0.0f);

	theta += 0.005f;

	EyeBufferType eyeBuffer;
	XMStoreFloat3(&eyeBuffer.eyePos, mCamera->GetPos());

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
	
	RenderMeshList(alphaMeshes, &lights, &matrices);

	//set shader for point sprite

	mMeshShaders[INSTANCE_SHADER]->PrepareShader(mD3D->GetDeviceContext());
	InstancedConstantsStruct constants;
	constants.matPtr = &matrices;
	constants.eyePtr = &eyeBuffer;
	result = mMeshShaders[INSTANCE_SHADER]->SetConstantShaderParameters((void*)&constants, context);

	ShaderPerMeshStruct perMesh;
	perMesh.sampler = mSamplerState;
	perMesh.diffuseSrv = mPointSprite->GetTexture();

	mMeshShaders[INSTANCE_SHADER]->SetPerMeshParameters(static_cast<void*>(&perMesh), context);
	
	unsigned int strides[4] = { sizeof(XMFLOAT3) , sizeof(XMFLOAT2), sizeof(XMFLOAT3), sizeof(XMFLOAT3)};
	XMFLOAT3 positions[2] = { lights.lightPos, XMFLOAT3(1.0f, 0.0f, 0.0f) };
	mPointSprite->Update(context, &positions[0]);

	ID3D11Buffer* buffers[4] = { mPointSprite->GetVertBuffer(), mPointSprite->GetUVBuffer(), mPointSprite->GetInstancePosBuffer() };
	mRenderer->DrawIndexedInstanced(&buffers[0], 4, strides, mPointSprite->GetIndexBuffer(), 6, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 1);

	context->OMSetBlendState(NULL, 0, 0xffffffff);

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