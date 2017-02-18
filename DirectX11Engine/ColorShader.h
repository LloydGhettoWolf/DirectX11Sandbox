#pragma once
//ColorShader.h
#include <d3d11.h>
//#include <d3dx11async.h>
#include <DirectXMath.h>

using namespace DirectX;

// Description:
// A Generic ShaderClass that every other shader class can derive from
// helps encapsulate tedious aspects, while giving flexibility to calling app
// regarding buffers to be passed in etc
struct MatrixBufferType
{
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;
};

class ShaderClass
{

public:
	ShaderClass() : mVertexShader(0),
					mPixelShader(0),
					mLayout(0),
					mMatrixBuffer(0) {};

	ShaderClass(const ShaderClass&) = delete;
	~ShaderClass() {};

	bool Init(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename, D3D11_INPUT_ELEMENT_DESC* inputs, int numInputs);
	void Shutdown();
	bool PrepareShader(ID3D11DeviceContext* deviceContext, MatrixBufferType *matrices);

private:
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename);

	bool SetShaderParameters(ID3D11DeviceContext *deviceContext, MatrixBufferType *matrices);

	ID3D11VertexShader* mVertexShader;
	ID3D11PixelShader* mPixelShader;
	ID3D11InputLayout* mLayout;
	ID3D11Buffer* mMatrixBuffer;

};
