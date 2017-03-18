#pragma once
//BaseApp.h abstract class for all apps
#include <dinput.h>

class D3DClass;

class BaseApp
{
public:
	BaseApp() : mD3D(nullptr) {};
	BaseApp(const BaseApp&) = delete;
	~BaseApp() {};

	virtual bool Init(int, int, HWND, HINSTANCE) = 0;
	virtual void Shutdown() = 0;
	virtual bool Frame(DIMOUSESTATE& state) = 0;

protected:
	virtual bool Render() = 0;
	virtual bool ReadInput(DIMOUSESTATE& state) = 0;

	D3DClass* mD3D;
	int mScreenWidth, mScreenHeight;
};
