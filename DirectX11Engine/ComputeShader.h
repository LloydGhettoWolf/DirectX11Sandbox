//ComputeShader.h
#pragma once
#include "SystemClass.h"

struct ID3D11Device;
struct ID3D11ComputeShader;
struct ID3D11DeviceContext;
struct ID3D11Buffer;
struct ID3D11ShaderResourceView;
struct ID3D11UnorderedAccessView;

class ComputeShader
{
	public:
		ComputeShader() : mComputeShader(0) {};
		ComputeShader(const ComputeShader&) = delete;
		virtual ~ComputeShader() { Shutdown(); };

		bool Init(ID3D11Device* device, HWND hwnd, LPCWSTR csFilename, void* inData, unsigned int numElems);
		void Shutdown();
		bool PrepareShader(ID3D11DeviceContext* deviceContext);
		bool CheckContents(ID3D11DeviceContext* deviceContext);
	private:
		ID3D11ComputeShader* mComputeShader;
		ID3D11Buffer* mInData;
		ID3D11Buffer* mOutData;
		ID3D11ShaderResourceView* mSrvIn;
		ID3D11UnorderedAccessView* mSrvOut;
};