#pragma once
//InputClass.h

class InputClass
{
	public:
		InputClass() {};
		InputClass(const InputClass&) = delete;
		~InputClass() {};

		bool Init();

		void KeyDown(unsigned int);
		void KeyUp(unsigned int);

		bool IsKeyDown(unsigned int);

	private:
		bool mKeys[256];
};