//InputClass.cpp

#include "InputClass.h"


bool InputClass::Init(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dInput, NULL);
	if (FAILED(result))
	{
		return false;
	}

	result = dInput->CreateDevice(GUID_SysMouse, &mMouse, NULL);
	if (FAILED(result))
	{
		return false;
	}

	result = mMouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		return false;
	}

	result = mMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	result = mMouse->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	// Initialize all the keys to being released and not pressed.
	for (int i = 0; i<256; i++)
	{
		mKeys[i] = false;
	}

	return true;
}

void InputClass::Update()
{
	HRESULT result;

	// Read the mouse device.
	result = mMouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mMouseState);

	if (FAILED(result))
	{
		// If the mouse lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			mMouse->Acquire();
		}
	}
}


void InputClass::KeyDown(unsigned int input)
{
	// If a key is pressed then save that state in the key array.
	mKeys[input] = true;
	return;
}


void InputClass::KeyUp(unsigned int input)
{
	// If a key is released then clear that state in the key array.
	mKeys[input] = false;
	return;
}


bool InputClass::IsKeyDown(unsigned int key)
{
	// Return what state the key is in (pressed/not pressed).
	return mKeys[key];
}
