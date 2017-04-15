#pragma once
//TestApp.h
#include <BaseApp.h>
#include <DirectXMath.h>
#include <vector>

class D3DClass;
class SimpleMesh;
class Camera;
class Frustum;
class Shader;
class OutlineShader;
class Texture;
struct ID3D11SamplerState;
struct ID3D11BlendState;
struct materialInfo;
class Renderer;
class ResourceAllocator;
class PointSprite;
struct LightPosBuffer;
struct MatrixBufferType;
struct processedMaterialInfo;

const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 3000.0f;
const float SCREEN_NEAR = 1.0f;
const bool FULL_SCREEN = false;

const int DIFF_SHADER = 0;
const int SPEC_SHADER = 1;
const int NORMMAP_SHADER = 2;
const int NORM_NO_SPEC_SHADER = 3;
const int MASK_SHADER = 4;
const int INSTANCE_SHADER = 5;

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
	bool PairMaterialsWithShaders(materialInfo* materials);
	bool Render();
	void RenderMeshList(vector<SimpleMesh*>& meshes, LightPosBuffer* lights, MatrixBufferType* matrices);
	bool ReadInput(DIMOUSESTATE& state);

	SimpleMesh* mMesh;
	PointSprite* mPointSprite;
	
	Renderer* mRenderer;
	ResourceAllocator* mResourceAllocator;
	
	Shader* mMeshShaders[INSTANCE_SHADER];
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

	XMMATRIX mWorld;

	unsigned int mNumMeshes = 0;
	unsigned int mNumMappedMeshes = 0;
	unsigned int mNumMaterials = 0;
	unsigned int mNumTextures = 0;
};
