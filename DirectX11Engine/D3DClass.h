#pragma once
//D3DClass.h
#include "SystemClass.h"

struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11Texture2D;
struct ID3D11DepthStencilState;
struct ID3D11DepthStencilView;
struct ID3D11RasterizerState;
class Camera;
class SimpleMesh;
class Shader;

using ShaderFuncPointer = void(*) (void*, ID3D11DeviceContext*);

class D3DClass
{
	public:
		D3DClass();
		D3DClass(const D3DClass&) = delete;
		~D3DClass() {};

		bool Init(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen,
			float screenDepth, float screenNear, int numSamples, int numRenderTargets);

		void Shutdown();

		void BeginScene(float red, float green, float blue, float alpha, bool depth);
		void EndScene();

		ID3D11ShaderResourceView *GetDepthTexture() { return mDepthSRV; }
		ID3D11ShaderResourceView *GetRenderTarget(int index) { return mRenderSRV[index];  }

		void SwitchOffDepthWrites();
		void SwitchOnDepthWrites();

		ID3D11Device*			GetDevice() { return mDevice; }
		ID3D11DeviceContext*	GetDeviceContext() { return mDeviceContext; }

		void SetRenderTarget(int index, bool depth);

		void RenderToFullScreenTriangle(Shader* shader, void* data);
	private:
		bool mVSyncEnabled;
		int mVideoCardMemory;
		char mVideoCardDescription[128];
		unsigned int mNumRenderTargets;

		IDXGISwapChain* mSwapChain;
		ID3D11Device* mDevice;
		ID3D11DeviceContext* mDeviceContext;

		ID3D11RenderTargetView** mRenderTargetView;
		ID3D11RenderTargetView* currentView;

		ID3D11Texture2D* mDepthStencilBuffer;
		ID3D11DepthStencilState* mDepthStencilState;
		ID3D11DepthStencilState* mDepthStencilStateAlpha;
		ID3D11DepthStencilView* mDepthStencilView;
		ID3D11RasterizerState* mRasterState;

		ID3D11ShaderResourceView* mDepthSRV;
		ID3D11ShaderResourceView** mRenderSRV;
};