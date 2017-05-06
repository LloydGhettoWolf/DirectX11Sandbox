#pragma once
#include <DirectXMath.h>
#include "Shader.h"

using namespace DirectX;

struct ID3D11Buffer;

class OrthoShader : public Shader
{
public:
	OrthoShader() {};
	~OrthoShader() { Shutdown(); }

	bool Init(ID3D11Device* device, HWND hwnd, unsigned int numLights);
	void Shutdown();
	bool PrepareShader(ID3D11DeviceContext* deviceContext);
	bool SetConstantShaderParameters(void* data, ID3D11DeviceContext* deviceContext);
	bool SetPerMeshParameters(void* data, ID3D11DeviceContext* deviceContext) { return true; }
private:
	ID3D11Buffer* mMatrixBuffer;

	struct MatrixBufferType
	{
		XMFLOAT4X4 orthoProjection;
	};
};