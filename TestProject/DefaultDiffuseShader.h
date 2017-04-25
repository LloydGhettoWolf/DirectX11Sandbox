#pragma once
//ColorShader.h
#include "Defines.h"
#include "Shader.h"

using namespace DirectX;


struct ID3D11Buffer;
struct ID3D11SamplerState;
struct ID3D11ShaderResourceView;
struct ID3D11DeviceContext;



class DefaultDiffuseShader : public Shader
{

public:
	DefaultDiffuseShader() : mMatrixBuffer(0),
							 mLightBuffer(0),
							 mMaterialBuffer(0){};

	DefaultDiffuseShader(const DefaultDiffuseShader&) = delete;
	~DefaultDiffuseShader() { Shutdown(); };

	bool Init(ID3D11Device* device, HWND hwnd,unsigned int numLights);
	void Shutdown();
	bool PrepareShader(ID3D11DeviceContext* deviceContext);
	bool SetConstantShaderParameters(void* data, ID3D11DeviceContext* deviceContext);
	bool SetPerMeshParameters(void* data, ID3D11DeviceContext* deviceContext);
private:

	ID3D11Buffer* mMatrixBuffer;
	ID3D11Buffer* mLightBuffer;
	ID3D11Buffer* mMaterialBuffer;

	int mNumLights;

	static const int MATRIX_BUFFER = 0;
	static const int LIGHT_BUFFER = 0;
	static const int MATERIAL_BUFFER = 1;

};
