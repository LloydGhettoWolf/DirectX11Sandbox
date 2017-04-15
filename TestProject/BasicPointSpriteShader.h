#pragma once
#include "Shader.h"

struct ID3D11Buffer;
struct ID3D11Device;
struct ID3D11SamplerState;
struct ID3D11ShaderResourceView;
struct ID3D11DeviceContext;

class PSShader : public Shader
{

public:
	PSShader() {};

	PSShader(const PSShader&) = delete;
	~PSShader() { Shutdown(); };

	bool Init(ID3D11Device* device, HWND hwnd);
	void Shutdown();
	bool PrepareShader(ID3D11DeviceContext* deviceContext);
	bool SetConstantShaderParameters(void* data, ID3D11DeviceContext* deviceContext);
	bool SetPerMeshParameters(void* data, ID3D11DeviceContext* deviceContext);

private:
	ID3D11Buffer* mMatrixBuffer;

	static const int MATRIX_BUFFER = 0;
};