#include "App.h"
#include <memory>
#include <algorithm>
#include <iostream>
#include <random>

constexpr float PI = 3.14159265f;
static int ballAngle = 0;
static int reflects;

App::App() : wnd(800, 600, "First try window") 
{
	for (int& sc : score)
		sc = 0;
	sticks.push_back(std::make_unique<Stick>(wnd.Gfx(),-1.1f, 0.5f));
	sticks.push_back(std::make_unique<Stick>(wnd.Gfx(), 1.1f, 0.5f));
	for (auto& st : sticks) {
		st->setSpeed(0.05f);
	}
	ballAngle = rand() % 360;
	ball = std::make_unique<Ball>(wnd.Gfx(), 0.5f, 0.5f);
	ball->setSpeed(1.f);
	ball->setAngle(ballAngle);
	wnd.Gfx().setProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
}

void App::initBall() {
	ballAngle = rand() % 360;
	ball.reset(new Ball(wnd.Gfx(), 0.5f, 0.5f));
	ball->setSpeed(1.f);
	ball->setAngle(ballAngle);
}

App::~App() {

}

int App::Go() 
{
	while (true) {
		if (const auto ecode = Window::processMessages()) {
			return *ecode;
		}
		doFrame();
	}
	return 0;
}



void App::doFrameSticks(const float dt, const AxisMotion& motion) {
	sticks[first]->setInpit(motion.Y[first]);
	sticks[second]->setInpit(motion.Y[second]);
	for (auto& stick : sticks) {
		if (stick->getInput() != 0.0f)
			stick->Update(dt);
		stick->Draw(wnd.Gfx());
		stick->Rect = DirectX::SimpleMath::Rectangle(
			wnd.getWinWidth() * (stick->getLocation().first - 0.06f),
			wnd.getWinHeight() * (stick->getLocation().second - 0.15f),
			0.06f * wnd.getWinWidth(),
			0.3 * wnd.getWinHeight()
		);
	}
}

void App::doFrameBall(const float dt) {
	ball->setAngle(ballAngle);
	ball->Update(dt);
	ball->Draw(wnd.Gfx());
	ballAngle = ballAngle % 360;
	ball->setSpeed(1.f + 0.05f * reflects);
	ball->Rect = DirectX::SimpleMath::Rectangle(
		wnd.getWinWidth() * (ball->getLocation().first - 0.07f),
		wnd.getWinHeight() * (ball->getLocation().second - 0.07f),
		0.07f * wnd.getWinWidth(),
		0.7f * wnd.getWinHeight()
	);
}

App::collisionType App::checkCollision() {
	if ((ball->Rect.y + ball->Rect.height) / wnd.getWinHeight() < -0.5f ||
		(ball->Rect.y) / wnd.getWinHeight() > 1.f)
		return border;
	if ((ball->Rect.x) / wnd.getWinWidth() < -1.15f)
		return border_left;
	if	((ball->Rect.x + ball->Rect.width) / wnd.getWinWidth() > 1.15f)
		return border_right;
	auto result = ball->Rect.Intersects(sticks[first]->Rect);
	if (result) return stick_1;
	result = ball->Rect.Intersects(sticks[second]->Rect);
	if (result) return stick_2;
	return none;
}

void App::doFrame() 
{
	const auto dt = timer.mark();
	wnd.Gfx().clearBuffer(0.07f, 0.1f, 0.12f);
	auto motion = getInput();
	auto collType = checkCollision();
	switch (collType) {
	case stick_1: 
	{
		if (ballAngle < 90 || ballAngle > 270) break;
		if (ballAngle < 180)
			ballAngle = 190 - ballAngle;
		else ballAngle = 550 - ballAngle;
		reflects++;
		break;
	}
	case stick_2: 
	{
		if (ballAngle > 90 && ballAngle < 270) break;
		if (ballAngle < 180)
			ballAngle = 190 - ballAngle;
		else ballAngle = ballAngle - 100;
		reflects++;
		break;
	}
	case border:
	{
		ballAngle = 360 - ballAngle;
		break;
	}
	case border_left:
	{
		score[second]++;
		initBall();
		reflects = 0;
	}
	case border_right:
	{
		score[first]++;
		initBall();
		reflects = 0;
	}
	case none:
		break;
	}
	doFrameSticks(dt, motion);
	doFrameBall(dt);
	wnd.Gfx().endFrame();
	std::string timerElapsed(std::to_string(1 / dt));
	wnd.setTitle(timerElapsed);
}

AxisMotion App::getInput() {
	AxisMotion motion;
	motion.Y[second] = static_cast<float>(wnd.kbd.keyIsPressed(VK_DOWN)) * stickSpeed -
		static_cast<float>(wnd.kbd.keyIsPressed(VK_UP)) * stickSpeed;
	motion.Y[first] = static_cast<float>(wnd.kbd.keyIsPressed(static_cast<unsigned char>(83))) * stickSpeed -
		static_cast<float>(wnd.kbd.keyIsPressed(static_cast<unsigned char>(87))) * stickSpeed;
	return std::move(motion);
}