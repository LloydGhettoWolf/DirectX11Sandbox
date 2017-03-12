#pragma once
//InputClass.h
#include <dinput.h>

class InputClass
{
	public:
		InputClass() {};
		InputClass(const InputClass&) = delete;
		~InputClass() {};

		bool Init(HINSTANCE hInstance, HWND hWnd);
		void Update();
		void KeyDown(unsigned int input);
		void KeyUp(unsigned int input);
		bool IsKeyDown(unsigned int key);
		const DIMOUSESTATE& GetMouseState() const { return mMouseState;  }

	private:
		IDirectInput8* dInput;
		IDirectInputDevice8* mMouse;
		DIMOUSESTATE mMouseState;

		unsigned int mKeys[256];
};