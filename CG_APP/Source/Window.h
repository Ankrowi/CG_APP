#pragma once

#include "WinDefs.h"
#include "Keyboard.h"
#include "Graphics.h"
#include <memory>
#include <optional>

class Window
{
	class WindowClass {
	public:
		static const char* getName() noexcept;
		static HINSTANCE getInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr const char* wndClassName = "duck";
		static WindowClass wndClass;
		HINSTANCE hInst;
	};
public:
	Window(int width, int height, const char* name);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	void setTitle(const std::string& title);
	static std::optional<int> processMessages();
	Graphics& Gfx();
private:
	static LRESULT CALLBACK handleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK handleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT handleMsg(HWND hWndm, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	int width;
	int height;
	HWND hWnd;
	std::unique_ptr<Graphics> pGfx;
public:
	Keyboard kbd;
};

