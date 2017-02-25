#pragma once
//SimpleApp.h
#include <dinput.h>
#include "OneLightShader.h"

class D3DClass;
class SimpleMesh;
class Camera;

const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;
const bool FULL_SCREEN = false;



class SimpleApp
{
	public:
		SimpleApp(): mD3D(nullptr),
					 mMesh(nullptr),
					 mShader(nullptr){};
		SimpleApp(const SimpleApp&) = delete;
		~SimpleApp() {};

		bool Init(int, int, HWND, HINSTANCE);
		void Shutdown();
		bool Frame();

	private:
		bool Render();
		bool ReadInput();

		D3DClass* mD3D;
		SimpleMesh* mMesh;
		OneLightShader* mShader;
		ID3D11SamplerState* mSamplerState;

		IDirectInput8* dInput;
		IDirectInputDevice8* mMouse;
		DIMOUSESTATE mMouseState;

		int mMouseX, mMouseY;

		int mScreenWidth, mScreenHeight;

		Camera* mCamera;

		XMMATRIX mWorld;
};
