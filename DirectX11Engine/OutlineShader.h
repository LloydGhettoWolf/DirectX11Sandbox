#pragma once
//outlineshader.h
#include <D3D11.h>
#include <DirectXMath.h>
#include "Defines.h"
#include "Shader.h"

using namespace DirectX;

class OutlineShader : public Shader
{

public:
	OutlineShader() : mMatrixBuffer(0) {};

	OutlineShader(const OutlineShader&) = delete;
	~OutlineShader() {};

	bool Init(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename, D3D11_INPUT_ELEMENT_DESC* inputs, int numElems);
	void Shutdown();
	bool PrepareShader(ID3D11DeviceContext* deviceContext);
	bool SetConstantShaderParameters(void* data, ID3D11DeviceContext* deviceContext);
	bool SetPerMeshParameters(void* data, ID3D11DeviceContext* deviceContext) { return true; }
private:
	ID3D11Buffer* mMatrixBuffer;
};