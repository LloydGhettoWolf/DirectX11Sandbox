#pragma once
#include "SystemClass.h"

struct ID3D11Device;
struct D3D11_INPUT_ELEMENT_DESC;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;
struct ID3D11DeviceContext;
struct ID3D10Blob;
struct D3D11_INPUT_ELEMENT_DESC;

class Shader
{
public:
	Shader() : mVertexShader(0),
		mPixelShader(0),
		mLayout(0)
	{};

	Shader(const Shader&) = delete;
	virtual ~Shader() {};

	virtual bool Init(ID3D11Device* device, HWND hwnd) = 0;
	virtual void Shutdown() = 0;
	virtual bool PrepareShader(ID3D11DeviceContext* deviceContext);
	virtual bool SetConstantShaderParameters(void* data, ID3D11DeviceContext* deviceContext) = 0;
	virtual bool SetPerMeshParameters(void* data, ID3D11DeviceContext* deviceContext) = 0;
protected:
	bool InitShaderData(ID3D11Device* device, HWND hwnd, LPCWSTR vsFilename, LPCWSTR psFilename, D3D11_INPUT_ELEMENT_DESC* inputs, int numElems);
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, LPCWSTR shaderFilename);

	void CreatePosNormUvLayout(D3D11_INPUT_ELEMENT_DESC* polygonLayout);
	void CreatePosNormUvTangentLayout(D3D11_INPUT_ELEMENT_DESC* polygonLayout);

	ID3D11VertexShader* mVertexShader;
	ID3D11PixelShader* mPixelShader;
	ID3D11InputLayout* mLayout;
};
