#pragma once
#include <windows.h>
#include <WinUser.h>
#include <iostream>

class WindowHandler
{
public:
	WindowHandler();
	~WindowHandler();
	void Init(LPCWSTR applicationName, HINSTANCE hInstance);
};

