#pragma once
//TestApp.h
#include <BaseApp.h>
#include "DefaultDiffuseShader.h"
#include <OutlineShader.h>
#include <ObjParser.h>
#include <Texture.h>
#include <dinput.h>

class D3DClass;
class SimpleMesh;
class Camera;
class Frustum;

const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 5000.0f;
const float SCREEN_NEAR = 0.1f;
const bool FULL_SCREEN = false;



class TestApp : public BaseApp
{
public:
	TestApp() : mMesh(nullptr), mShader(nullptr) {};
	TestApp(const TestApp&) = delete;
	~TestApp() {};

	bool Init(int, int, HWND, HINSTANCE);
	void Shutdown();
	bool Frame(DIMOUSESTATE& state);
	


private:
	bool Render();
	bool ReadInput(DIMOUSESTATE& state);

	SimpleMesh* mMesh;
	DefaultDiffuseShader* mShader;
	OutlineShader* mOutlineShader;
	ID3D11SamplerState* mSamplerState;

	materialInfo* mMaterialProperties;

	Texture* mTextures;

	int mMouseRotateX, mMouseRotateY;
	int mMouseVertY;
	int mMouseHorizZ;

	int mScreenWidth, mScreenHeight;

	Camera* mCamera;
	Frustum* mFrustum;

	XMMATRIX mWorld;

	unsigned int mNumMeshes = 0;
	unsigned int mNumMaterials = 0;
	unsigned int mNumTextures = 0;
};
