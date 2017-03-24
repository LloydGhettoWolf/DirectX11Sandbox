#pragma once
//ColorShader.h
#include <DirectXMath.h>
#include "Defines.h"
#include "Shader.h"

using namespace DirectX;

static const int MATRIX_BUFFER = 0;
static const int LIGHT_BUFFER = 0;
static const int MATERIAL_BUFFER = 1;

// Description:
// A

struct DiffuseShaderPerMeshStruct
{
	MaterialProperties* material;
	ID3D11SamplerState* sampler;
	ID3D11ShaderResourceView* srv;
};


class DefaultDiffuseShader : public Shader
{

public:
	DefaultDiffuseShader() : mMatrixBuffer(0),
							 mLightBuffer(0),
							 mMaterialBuffer(0){};

	DefaultDiffuseShader(const DefaultDiffuseShader&) = delete;
	~DefaultDiffuseShader() {};

	bool Init(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename, D3D11_INPUT_ELEMENT_DESC* inputs, int numElems);
	void Shutdown();
	bool PrepareShader(ID3D11DeviceContext* deviceContext);
	bool SetConstantShaderParameters(void* data, ID3D11DeviceContext* deviceContext);
	bool SetPerMeshParameters(void* data, ID3D11DeviceContext* deviceContext);
private:

	ID3D11Buffer* mMatrixBuffer;
	ID3D11Buffer* mLightBuffer;
	ID3D11Buffer* mMaterialBuffer;
};
