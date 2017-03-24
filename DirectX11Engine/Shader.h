#pragma once
#include <D3D11.h>
class Shader
{
public:
	Shader() : mVertexShader(0),
		mPixelShader(0),
		mLayout(0)
	{};

	Shader(const Shader&) = delete;
	~Shader() {};

	virtual bool Init(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename, D3D11_INPUT_ELEMENT_DESC* inputs, int numElems) = 0;
	virtual void Shutdown() = 0;
	virtual bool PrepareShader(ID3D11DeviceContext* deviceContext);
	virtual bool SetConstantShaderParameters(void* data, ID3D11DeviceContext* deviceContext) = 0;
	virtual bool SetPerMeshParameters(void* data, ID3D11DeviceContext* deviceContext) = 0;
protected:
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename);

	ID3D11VertexShader* mVertexShader;
	ID3D11PixelShader* mPixelShader;
	ID3D11InputLayout* mLayout;
};
