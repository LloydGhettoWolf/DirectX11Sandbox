#pragma once
//outlineshader.h
#include <D3D11.h>
#include <DirectXMath.h>
#include "Defines.h"

using namespace DirectX;

class OutlineShader
{

public:
	OutlineShader() : mVertexShader(0),
		mPixelShader(0),
		mLayout(0),
		mMatrixBuffer(0) 
	{};

	OutlineShader(const OutlineShader&) = delete;
	~OutlineShader() {};

	bool Init(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename, D3D11_INPUT_ELEMENT_DESC* inputs, int numElems);
	void Shutdown();
	bool PrepareShader(ID3D11DeviceContext* deviceContext);
	bool SetConstantShaderParameters(ID3D11DeviceContext* deviceContext, MatrixBufferType *matrices);

private:
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename);

	ID3D11VertexShader* mVertexShader;
	ID3D11PixelShader* mPixelShader;
	ID3D11InputLayout* mLayout;

	ID3D11Buffer* mMatrixBuffer;
};