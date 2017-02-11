#pragma once
//GraphicsClass.h
#include <Windows.h>
#include "D3DClass.h"

const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;
const bool FULL_SCREEN = false;

class GraphicsClass
{
	public:
		GraphicsClass(): mD3D(nullptr) {};
		GraphicsClass(const GraphicsClass&) = delete;
		~GraphicsClass() {};

		bool Init(int, int, HWND);
		void Shutdown();
		bool Frame();

	private:
		bool Render();

		D3DClass* mD3D;
};
