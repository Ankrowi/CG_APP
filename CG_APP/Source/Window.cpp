#include "Window.h"
#include "WinUser.h"
#include <iostream>

Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept : 
	hInst(GetModuleHandle(nullptr)) 
{
	WNDCLASSEX wc;
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = handleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = getInstance();
	wc.hIcon = nullptr;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = getName();
	wc.hIconSm = nullptr;
	wc.cbSize = sizeof(WNDCLASSEX);
	auto res = RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass() 
{
	UnregisterClass(wndClassName, getInstance());
}

const char* Window::WindowClass::getName()  noexcept 
{
	return wndClassName;
}

HINSTANCE Window::WindowClass::getInstance() noexcept 
{
	return wndClass.hInst;
}

Window::Window(int width, int height, const char* name) :
	width(width), height(height) 
{
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);
	// create window & get hWnd
	hWnd = CreateWindowEx(WS_EX_APPWINDOW,
	//hWnd = CreateWindow(
		WindowClass::getName(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, WindowClass::getInstance(), this
	);
	// check for error
	if (hWnd == nullptr)
	{
		throw "some trouble";
	}
	// newly created windows start off as hidden
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	// create graphics object
	pGfx = std::make_unique<Graphics>(hWnd);
}

Window::~Window() 
{
	DestroyWindow(hWnd);
}

void Window::setTitle(const std::string& title)
{
	SetWindowText(hWnd, title.c_str());
}

std::optional<int> Window::processMessages()
{
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) {
			return (int)msg.wParam;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return {};
}

Graphics& Window::Gfx()
{
	return *pGfx;
}

LRESULT Window::handleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (msg == WM_NCCREATE) {
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::handleMsgThunk));
		return pWnd->handleMsg(hWnd, msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Window::handleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return pWnd->handleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::handleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg) {
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
	{
		// If a key is pressed send it to the input object so it can record that state.
		std::cout << "Key: " << static_cast<unsigned int>(wParam) << std::endl;
		if (static_cast<unsigned int>(wParam) == 27) PostQuitMessage(0);
		if (static_cast<unsigned int>(wParam) == 87 ||
			static_cast<unsigned int>(wParam) == 83 ||
			static_cast<unsigned int>(wParam) == VK_UP ||
			static_cast<unsigned int>(wParam) == VK_DOWN) 
		{
			kbd.onKeyPressed(wParam);
		}
		return 0;
	}
	case WM_KEYUP:
	{
		std::cout << "Key: " << static_cast<unsigned int>(wParam) << std::endl;
		if (static_cast<unsigned int>(wParam) == 27) PostQuitMessage(0);
		if (static_cast<unsigned int>(wParam) == 87 ||
			static_cast<unsigned int>(wParam) == 83 ||
			static_cast<unsigned int>(wParam) == VK_UP ||
			static_cast<unsigned int>(wParam) == VK_DOWN)
		{
			kbd.onKeyReleased(wParam);
		}
		return 0;
	}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}


float Window::getWinHeight() {
	return height;
}

float Window::getWinWidth() {
	return width;
}