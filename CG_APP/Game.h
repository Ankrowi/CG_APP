#pragma once

#include "WindowHandler.h"

class Game
{
public:
	static Game* Instance;
	~Game();
	void Initialise();
	void Update();
	void run();
	LRESULT InputHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam) {
		return WndProc(hwnd, umessage, wparam, lparam);
	}
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
	{
		switch (umessage)
		{
		case WM_KEYDOWN:
		{
			// If a key is pressed send it to the input object so it can record that state.
			std::cout << "Key: " << static_cast<unsigned int>(wparam) << std::endl;

			if (static_cast<unsigned int>(wparam) == 27) PostQuitMessage(0);
			return 0;
		}
		default:
		{
			return DefWindowProc(hwnd, umessage, wparam, lparam);
		}
		}
	}
private:
	Game();
};

