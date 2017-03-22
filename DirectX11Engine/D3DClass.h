#pragma once
//D3DClass.h
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include "SimpleMesh.h"
#include "Camera.h"

using namespace DirectX;

using ShaderFuncPointer = void(*) (void*, ID3D11DeviceContext*);

class D3DClass
{
	public:
		D3DClass();
		D3DClass(const D3DClass&) = delete;
		~D3DClass() {};

		bool Init(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen,
			float screenDepth, float screenNear);

		void Shutdown();

		void BeginScene(float red, float green, float blue, float alpha);
		void EndScene();

		void RenderOpaque(SimpleMesh* meshes, unsigned int numMeshes, Camera* cam);
		//sort along z

		ID3D11Device*			GetDevice() { return mDevice; }
		ID3D11DeviceContext*	GetDeviceContext() { return mDeviceContext; }

		void GetVideoCardInfo(char* cardName, int& memory, HWND hWnd);
		
	private:
		bool mVSyncEnabled;
		int mVideoCardMemory;
		char mVideoCardDescription[128];

		IDXGISwapChain* mSwapChain;
		ID3D11Device* mDevice;
		ID3D11DeviceContext* mDeviceContext;
		ID3D11RenderTargetView* mRenderTargetView;
		ID3D11Texture2D* mDepthStencilBuffer;
		ID3D11DepthStencilState* mDepthStencilState;
		ID3D11DepthStencilView* mDepthStencilView;
		ID3D11RasterizerState* mRasterState;
};