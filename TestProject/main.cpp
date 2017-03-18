//main.cpp
#include "TestApp.h"
#include "SystemClass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR pScmdline, int iCmdShow) 
{
	SystemClass* sysClass;
	BaseApp* app;
	bool result;

	sysClass = new SystemClass();

	if (!sysClass) 
	{
		return 1;
	}

	app = new TestApp();

	if (!app)
	{
		return 1;
	}

	result = sysClass->Init(app, 800, 600);

	if (!result)
	{
		return 1;
	}
	else
	{
		sysClass->Run();
	}

	sysClass->Shutdown();
	delete sysClass;

	return 0;

}