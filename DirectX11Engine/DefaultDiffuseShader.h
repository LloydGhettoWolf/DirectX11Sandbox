#pragma once
//ColorShader.h
#include <D3D11.h>
//#include <d3dx11async.h>
#include <DirectXMath.h>

using namespace DirectX;

// Description:
// A
struct MatrixBufferType
{
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;
};

struct LightPosBuffer
{
	XMFLOAT3 lightPos;
	float padding1;
	XMFLOAT4 lightCol;
};

struct MaterialProperties
{
	XMFLOAT4 diffuseCol;
	XMFLOAT4 specCol;
	float specComponent;
	float padding[3];
};

struct PerMeshStruct
{
	MaterialProperties* material;
	ID3D11SamplerState* sampler;
	ID3D11ShaderResourceView* srv;
};

class DefaultDiffuseShader
{

public:
	DefaultDiffuseShader() : mVertexShader(0),
					mPixelShader(0),
					mLayout(0),
					mMatrixBuffer(0) {};

	DefaultDiffuseShader(const DefaultDiffuseShader&) = delete;
	~DefaultDiffuseShader() {};

	bool Init(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename, D3D11_INPUT_ELEMENT_DESC* inputs, int numElems);
	void Shutdown();
	bool PrepareShader(ID3D11DeviceContext* deviceContext);
	bool SetConstantShaderParameters(ID3D11DeviceContext* deviceContext, MatrixBufferType *matrices, LightPosBuffer* lights);
	bool SetPerMeshParamaters(void* data, ID3D11DeviceContext* deviceContext);
private:
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename);


	ID3D11VertexShader* mVertexShader;
	ID3D11PixelShader* mPixelShader;
	ID3D11InputLayout* mLayout;
	ID3D11Buffer* mMatrixBuffer;
	ID3D11Buffer* mLightBuffer;
	ID3D11Buffer* mMaterialBuffer;
};
