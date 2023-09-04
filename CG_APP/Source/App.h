#pragma once

#include "Window.h"
#include "Timer.h"
#include "Ball.h"
#include "Stick.h"
#include "Drawable.h"
#include "Box.h"

struct AxisMotion
{
	float Y[2];
};

class App
{
public:
	App();
	~App();
	int Go();
private:
	void doFrame();
	AxisMotion getInput();
	enum Players {
		first,
		second
	};
	enum collisionType {
		border,
		border_left,
		border_right,
		stick_1,
		stick_2,
		none
	};
	void doFrameSticks(const float dt, const AxisMotion& motion);
	void doFrameBall(const float dt);
	collisionType checkCollision();
	void initBall();
private:
	Window wnd;
	Timer timer;
	std::vector<std::unique_ptr<Stick>> sticks;
	int score[2];
	std::unique_ptr<Ball> ball;
	std::vector<std::unique_ptr<Drawable>> drawables;
	float stickSpeed = 30.0f;
};

