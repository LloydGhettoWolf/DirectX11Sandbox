#pragma once
#pragma once
//NormalMapShader.h
#pragma once
#include "Defines.h"
#include "NormalMapShader.h"

struct ID3D11Buffer;
struct ID3D11SamplerState;
struct ID3D11ShaderResourceView;
struct ID3D11DeviceContext;

class NormNoSpecMapShader : public NormalMapShader
{
public:
	NormNoSpecMapShader() {};

	NormNoSpecMapShader(const NormNoSpecMapShader&) = delete;
	~NormNoSpecMapShader() { Shutdown(); }

	bool Init(ID3D11Device* device, HWND hwnd);
	bool SetPerMeshParameters(void* data, ID3D11DeviceContext* deviceContext);
};
