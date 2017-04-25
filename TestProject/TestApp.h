#pragma once
//TestApp.h
#include <BaseApp.h>
#include <DirectXMath.h>
#include <vector>
#include <chrono>

class D3DClass;
class SimpleMesh;
class Camera;
class Frustum;
class Shader;
class OutlineShader;
class Texture;
struct ID3D11SamplerState;
struct ID3D11BlendState;
struct ID3D11ShaderResourceView;
struct materialInfo;
class Renderer;
class ResourceAllocator;
class PointSprite;
struct LightPosBuffer;
struct MatrixBufferType;
struct EyeBufferType;
struct processedMaterialInfo;

const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 3000.0f;
const float SCREEN_NEAR = 10.0f;
const bool FULL_SCREEN = false;
const float SCREEN_WIDTH = 800;
const float SCREEN_HEIGHT = 600;
const int NUM_SAMPLES = 8;
const int NUM_RENDER_TARGETS = 2;

const int NUM_LIGHTS = 400;

enum ShaderTypes{ DIFF_SHADER = 0,
				  SPEC_SHADER,
				  NORMMAP_SHADER,
				  NORM_NO_SPEC_SHADER,
				  MASK_SHADER,
				  INSTANCE_SHADER,
				  FULL_SCREEN_SHADER
				};


const float VERT_MOVEMENT_SPEED = 10.0f;
const float FORWARD_MOVEMENT_SPEED = 20.0f;

const float FRAMES_PER_SEC = 60.0f;
//ideal delta time
const float DELTA_TIME = 1.0f / FRAMES_PER_SEC;

using namespace DirectX;
using namespace std;

class TestApp : public BaseApp
{
public:
	TestApp() : mMesh(nullptr) {};
	TestApp(const TestApp&) = delete;
	~TestApp() {};

	bool Init(int, int, HWND, HINSTANCE);
	void Shutdown();
	bool Frame(DIMOUSESTATE& state);

private:
	bool InitLights();
	bool PairMaterialsWithShaders(materialInfo* materials);
	bool Render();
	void RenderMeshList(vector<SimpleMesh*>& meshes, LightPosBuffer* lights, MatrixBufferType* matrices, EyeBufferType* eye);
	bool ReadInput(DIMOUSESTATE& state);

	SimpleMesh* mMesh;
	PointSprite* mPointSprite;
	
	Renderer* mRenderer;
	ResourceAllocator* mResourceAllocator;
	
	Shader* mMeshShaders[FULL_SCREEN_SHADER+1];
	OutlineShader* mOutlineShader;
	ID3D11SamplerState* mSamplerState;
	ID3D11BlendState* mBlendState;

	processedMaterialInfo* mMaterials;

	Texture* mTextures;

	int mMouseRotateX, mMouseRotateY;
	int mMouseVertY;
	int mMouseHorizZ;

	int mScreenWidth, mScreenHeight;

	Camera* mCamera;
	Frustum* mFrustum;

	XMFLOAT4X4 mWorld;
	LightPosBuffer* mLights;
	XMFLOAT4* lightsStartPos;

	unsigned int mNumMeshes = 0;
	unsigned int mNumMappedMeshes = 0;
	unsigned int mNumMaterials = 0;
	unsigned int mNumTextures = 0;

	chrono::high_resolution_clock::time_point mLastTime;
};
