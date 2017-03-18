//main.cpp

#include "SystemClass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR pScmdline, int iCmdShow) 
{
	SystemClass* sysClass;
	bool result;

	sysClass = new SystemClass();

	if (!sysClass) 
	{
		return 1;
	}

	result = sysClass->Init(800, 600);

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