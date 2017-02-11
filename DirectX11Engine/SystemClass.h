//SystemClass.h
#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "InputClass.h"
#include "GraphicsClass.h"

class SystemClass
{
public:
	SystemClass(bool fullScreen = false) : mAppName(L"DirectX11App"), mInput(nullptr), mGraphics(nullptr), mFullScreen(fullScreen){};
	SystemClass(const SystemClass&) = delete;
	~SystemClass() {};

	bool Init();
	void Run();
	void Shutdown();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

	LPCWSTR mAppName;
	HINSTANCE mInstance;
	HWND mHwnd;
	bool mFullScreen;

	InputClass* mInput;
	GraphicsClass* mGraphics;
};

static SystemClass* App;

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
