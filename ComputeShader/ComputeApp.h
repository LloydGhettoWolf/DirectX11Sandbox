#pragma once
#include <BaseApp.h>

class ComputeShader;

class ComputeApp : public BaseApp
{
public:
	ComputeApp() {};
	ComputeApp(const ComputeApp&) = delete;
	~ComputeApp() { Shutdown(); };

	bool Init(int, int, HWND, HINSTANCE);
	void Shutdown();
	bool Frame(DIMOUSESTATE& state);
private:
	bool Render() { return true; };
	bool ReadInput(DIMOUSESTATE& state) { return true; };

	ComputeShader* mCompShader;
};

