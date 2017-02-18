#pragma once
//SimpleApp.h

#include "ColorShader.h"

class D3DClass;
class SimpleMesh;

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

		bool Init(int, int, HWND);
		void Shutdown();
		bool Frame();

	private:
		bool Render();

		D3DClass* mD3D;
		SimpleMesh* mMesh;
		ShaderClass* mShader;

		XMMATRIX mProj;
		XMMATRIX mWorld;
		XMMATRIX mView;
};
