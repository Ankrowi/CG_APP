#pragma once

#include "Window.h"
#include "Timer.h"
#include "Drawable.h"

class App
{
public:
	App();
	~App() = default;
	int Go();
private:
	void doFrame();
private:
	Window wnd;
	Timer timer;
	std::vector<std::unique_ptr<Drawable>> drawables;
	static constexpr size_t nDrawables = 50;
};

