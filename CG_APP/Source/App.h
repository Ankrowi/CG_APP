#pragma once

#include "Window.h"
#include "Timer.h"
#include "Drawable.h"
#include "Camera.h"

class App
{
public:
	App();
	~App() = default;
	int Go();
private:
	void doFrame();
	void getInput();
	void updateCamera();
private:
	Window wnd;
	Timer timer;
	Camera camera;
	std::vector<std::unique_ptr<Drawable>> drawables;
	static constexpr size_t nDrawables = 50;

};

