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
class OrthoShader;
class Texture;
class Renderer;
class ResourceAllocator;
class PointSprite;
class TextBox;

struct ID3D11SamplerState;
struct ID3D11BlendState;
struct ID3D11ShaderResourceView;
struct LightPosBuffer;
struct MatrixBufferType;
struct EyeBufferType;
struct materialInfo;
struct processedMaterialInfo;

const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 3000.0f;
const float SCREEN_NEAR = 10.0f;
const bool FULL_SCREEN = false;
const float SCREEN_WIDTH = 800;
const float SCREEN_HEIGHT = 600;
const int NUM_SAMPLES = 4;
const int NUM_RENDER_TARGETS = 1;

const int NUM_LIGHTS = 400;

enum ShaderTypes{ DIFF_SHADER = 0,
				  SPEC_SHADER,
				  NORMMAP_SHADER,
				  NORM_NO_SPEC_SHADER,
				  MASK_SHADER,
				  INSTANCE_SHADER,
				  FULL_SCREEN_SHADER,
				  LAST_SHADER
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
	~TestApp() { Shutdown(); };

	bool Init(int, int, HWND, HINSTANCE);
	void Shutdown();
	bool Frame(DIMOUSESTATE& state);

private:
	bool InitLights();
	bool PairMaterialsWithShaders(materialInfo* materials);
	bool Render();
	void RenderMeshList(vector<SimpleMesh*>& meshes, LightPosBuffer* lights, MatrixBufferType* matrices, EyeBufferType* eye);
	void RenderUI();
	bool ReadInput(DIMOUSESTATE& state);

	SimpleMesh* mMesh;
	PointSprite* mPointSprite;
	
	Renderer* mRenderer;
	ResourceAllocator* mResourceAllocator;
	
	Shader* mMeshShaders[FULL_SCREEN_SHADER+1];
	OrthoShader* mOrthoShader;
	OutlineShader* mOutlineShader;
	ID3D11SamplerState* mSamplerState;
	ID3D11BlendState* mBlendState;

	processedMaterialInfo* mMaterials;

	Texture* mTextures;
	Texture* mFontTexture;
	TextBox* mScreenRect;

	int mMouseRotateX, mMouseRotateY;
	int mMouseVertY;
	int mMouseHorizZ;

	int mScreenWidth, mScreenHeight;

	Camera* mCamera;
	Frustum* mFrustum;

	char* mOutputMessage;

	XMFLOAT4X4 mWorld, mOrtho;
	LightPosBuffer* mLights;
	XMFLOAT4* lightsStartPos;

	unsigned int mNumMeshes = 0;
	unsigned int mNumMappedMeshes = 0;
	unsigned int mNumMaterials = 0;
	unsigned int mNumTextures = 0;

	chrono::high_resolution_clock::time_point mLastTime;

	std::vector<double> secs;
};
