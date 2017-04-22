#pragma once
#include "Defines.h"
#include <Shader.h>

struct ID3D11Buffer;
struct ID3D11SamplerState;
struct ID3D11ShaderResourceView;
struct ID3D11DeviceContext;

class SimpleFullScreenShader : public Shader
{
public:
	SimpleFullScreenShader() {};

	SimpleFullScreenShader(const SimpleFullScreenShader&) = delete;
	~SimpleFullScreenShader() { Shutdown(); }

	bool Init(ID3D11Device* device, HWND hwnd, unsigned int numLights);
	void Shutdown();
	bool PrepareShader(ID3D11DeviceContext* deviceContext);
	bool SetConstantShaderParameters(void* data, ID3D11DeviceContext* deviceContext);
	bool SetPerMeshParameters(void* data, ID3D11DeviceContext* deviceContext);

protected:
	bool InitSimpleFullScreenShader(ID3D11Device* device, HWND hwnd, LPCWSTR* vsName, LPCWSTR* psName);

};
