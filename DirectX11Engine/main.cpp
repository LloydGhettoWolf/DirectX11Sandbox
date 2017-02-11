//main.cpp

#include "SystemClass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR pScmdline, int iCmdShow) 
{
	SystemClass* sysClass;
	bool result;

	sysClass = new SystemClass(true);

	if (!sysClass) 
	{
		return 1;
	}

	result = sysClass->Init();

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