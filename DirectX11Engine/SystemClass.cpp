//SystemClass.cpp
#include "SystemClass.h"
#include "InputClass.h"
#include "BaseApp.h"

bool SystemClass::Init(BaseApp* app, int width, int height)
{

	InitializeWindows(width, height);

	mInput = new InputClass();
	if(!mInput)
	{
		return false;
	}

	if (!mInput->Init(mInstance, mHwnd)) 
	{
		return false;
	}

	mApp = app;
	if (!mApp)
	{
		return false;
	}

	if (!mApp->Init(width, height, mHwnd, mInstance))
	{
		return false;
	}

	return true;
}

void SystemClass::Shutdown() 
{
	if (mInput)
	{
		delete mInput;
	}

	if (mApp)
	{
		mApp->Shutdown();
		delete mApp;
	}

	ShutdownWindows();
}

void SystemClass::Run() 
{
	MSG msg;
	bool done, result;

	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	done = false;
	while (!done)
	{
		// Handle the windows messages.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// Otherwise do the frame processing.
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}

	}

	return;
}

bool SystemClass::Frame()
{
	bool result;

	// Check if the user pressed escape and wants to exit the application.
	if (mInput->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	mInput->Update();
	DIMOUSESTATE state = mInput->GetMouseState();

	// Do the frame processing for the graphics object.
	result = mApp->Frame(state);

	if (!result)
	{
		return false;
	}

	return true;
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		// Check if a key has been pressed on the keyboard.
		case WM_KEYDOWN:
			// If a key is pressed send it to the input object so it can record that state.
			mInput->KeyDown((unsigned int)wparam);
			return 0;
		// Check if a key has been released on the keyboard.
		case WM_KEYUP:
			// If a key is released then send it to the input object so it can unset the state for that key.
			mInput->KeyUp((unsigned int)wparam);
			return 0;
		// Any other messages send to the default message handler as our application won't make use of them.
		default:
			return DefWindowProc(hwnd, umsg, wparam, lparam);
	
	}
}

void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	systemApp = this;

	// Get the instance of this application.
	mInstance = GetModuleHandle(NULL);

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = mInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = mAppName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	//screenWidth = GetSystemMetrics(SM_CXSCREEN);
	//screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	//if (mFullScreen)
	//{
	//	// If full screen set the screen to maximum size of the users desktop and 32bit.
	//	memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
	//	dmScreenSettings.dmSize = sizeof(dmScreenSettings);
	//	dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
	//	dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
	//	dmScreenSettings.dmBitsPerPel = 32;
	//	dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

	//	// Change the display settings to full screen.
	//	ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

	//	// Set the position of the window to the top left corner.
	//	posX = posY = 0;
	//}
	//else
	//{

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	//}

	// Create the window with the screen settings and get the handle to it.
	mHwnd = CreateWindowEx(WS_EX_APPWINDOW, mAppName, mAppName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, mInstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(mHwnd, SW_SHOW);
	SetForegroundWindow(mHwnd);
	SetFocus(mHwnd);

	// Hide the mouse cursor.
	ShowCursor(false);

	return;
}

void SystemClass::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if (mFullScreen)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(mHwnd);
	mHwnd = NULL;

	// Remove the application instance.
	UnregisterClass(mAppName, mInstance);
	mInstance = NULL;

	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if the window is being destroyed.
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		// Check if the window is being closed.
		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;
		// All other messages pass to the message handler in the system class.
		default:
			return systemApp->MessageHandler(hwnd, umessage, wparam, lparam);
	}
}
