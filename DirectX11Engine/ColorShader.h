#pragma once
//ColorShader.h
#include <d3d11.h>
#include <d3dx11async.h>
#include <DirectXMath.h>

using namespace DirectX;

class ColorShaderClass
{

public:
	ColorShaderClass() : mVertexShader(0),
						 mPixelShader(0),
						 mLayout(0),
						 mMatrixBuffer(0) {};

	ColorShaderClass(const ColorShaderClass&) = delete;
	~ColorShaderClass() {};

	bool Initialize(ID3D11Device* device, HWND hwnd);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, const XMMATRIX& worldMatrix,
				const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix);

private:
	bool InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& worldMatrix,
		const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

	ID3D11VertexShader* mVertexShader;
	ID3D11PixelShader* mPixelShader;
	ID3D11InputLayout* mLayout;
	ID3D11Buffer* mMatrixBuffer;

	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};
};