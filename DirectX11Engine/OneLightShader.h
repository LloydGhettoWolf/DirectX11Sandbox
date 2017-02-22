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
	XMFLOAT3 lightCol;
	float padding2;
};

class OneLightShader
{

public:
	OneLightShader() : mVertexShader(0),
					mPixelShader(0),
					mLayout(0),
					mMatrixBuffer(0) {};

	OneLightShader(const OneLightShader&) = delete;
	~OneLightShader() {};

	bool Init(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename, D3D11_INPUT_ELEMENT_DESC* inputs, int numElems);
	void Shutdown();
	bool PrepareShader(ID3D11DeviceContext* deviceContext, MatrixBufferType *matrices, LightPosBuffer* lights, ID3D11ShaderResourceView* srv, ID3D11SamplerState* samplerState);

private:
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename);

	bool SetShaderParameters(ID3D11DeviceContext *deviceContext, MatrixBufferType *matrices, LightPosBuffer* lights, ID3D11ShaderResourceView* srv);

	ID3D11VertexShader* mVertexShader;
	ID3D11PixelShader* mPixelShader;
	ID3D11InputLayout* mLayout;
	ID3D11Buffer* mMatrixBuffer;
	ID3D11Buffer* mLightBuffer;
};
