#include "App.h"
#include "Box.h"
#include "Sphere.h"
#include <memory>
#include <algorithm>

constexpr float PI = 3.14159265f;
constexpr float CameraSpeed = 0.1f;

App::App() : wnd(800, 600, "First try window") {
	class Factory
	{
	public:
		Factory(Graphics& gfx)
			:
			gfx(gfx)
		{}
		std::unique_ptr<Drawable> operator()()
		{
			switch (typedist(rng))
			{
			case 1:
				return std::make_unique<Box>(
					gfx, rng, adist, ddist,
					odist, rdist, bdist
				);
			case 0:
				return std::make_unique<Sphere>(
					gfx, rng, adist, ddist,
					odist, rdist, latdist, longdist
				);
			default:
				//assert(false && "bad drawable type in factory");
				return std::make_unique<Box>(
					gfx, rng, adist, ddist,
					odist, rdist, bdist
				);
			}
		}
	private:
		Graphics& gfx;
		std::mt19937 rng{ std::random_device{}() };
		std::uniform_real_distribution<float> adist{ 0.0f,PI * 2.0f };
		std::uniform_real_distribution<float> ddist{ 0.0f,PI * 0.5f };
		std::uniform_real_distribution<float> odist{ 0.0f,PI * 0.08f };
		std::uniform_real_distribution<float> rdist{ 6.0f,20.0f };
		std::uniform_real_distribution<float> bdist{ 0.4f,3.0f };
		std::uniform_int_distribution<int> latdist{ 5,20 };
		std::uniform_int_distribution<int> longdist{ 8,40 };
		std::uniform_int_distribution<int> typedist{ 0,1 };
	};

	Factory f(wnd.Gfx());
	drawables.reserve(nDrawables);
	std::generate_n(std::back_inserter(drawables), nDrawables, f);
	//wnd.Gfx().setProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
	//wnd.Gfx().setCamera(DirectX::XMMatrixTranslation(0.f, 0.f, 1.f));
	
	camera.InitProjMatrix(1.0f, 600, 800, 0.5f, 400.0f);
	camera.Move(DirectX::XMFLOAT3(0.0f, 0.0f, -10.f));
	auto cProj = camera.Proj();
	wnd.Gfx().setProjection(
		DirectX::XMLoadFloat4x4(&cProj)
	);
	updateCamera();
}

int App::Go() {
	while (true) {
		if (const auto ecode = Window::processMessages()) {
			return *ecode;
		}
		doFrame();
	}
	return 0;
}

void App::doFrame() {
	const auto dt = timer.mark();
	wnd.Gfx().clearBuffer(0.07f, 0.0f, 0.12f);
	getInput();
	updateCamera();
	for (auto& d : drawables) {
		d->Update(dt);
		d->Draw(wnd.Gfx());
	}
	wnd.Gfx().endFrame();
	std::string timerElapsed(std::to_string(1 / dt));
	wnd.setTitle(timerElapsed);
}

std::pair<int, int> prevCameraPos = { 1, 1 };

void App::getInput() 
{
	using namespace DirectX;
	if (wnd.kbd.keyIsPressed(83))
	{
		auto fMove = camera.LookAtTarget();
		auto vMove = -CameraSpeed * XMLoadFloat3(&fMove);
		XMStoreFloat3(
			&fMove,
			vMove
		);
		camera.Move(fMove);
	}
	if (wnd.kbd.keyIsPressed(87))
	{
		auto fMove = camera.LookAtTarget();
		auto vMove = CameraSpeed * XMLoadFloat3(&fMove);
		XMStoreFloat3(
			&fMove,
			vMove
		);
		camera.Move(fMove);
	}
	if (wnd.kbd.keyIsPressed(65))
	{
		auto fMove = camera.LookAtTarget();
		auto vMove = -CameraSpeed * XMVector3Orthogonal(XMLoadFloat3(&fMove));
		XMStoreFloat3(
			&fMove,
			vMove
		);
		camera.Move(fMove);
	}
	if (wnd.kbd.keyIsPressed(68))
	{
		auto fMove = camera.LookAtTarget();
		auto vMove = CameraSpeed * XMVector3Orthogonal(XMLoadFloat3(&fMove));
		XMStoreFloat3(
			&fMove,
			vMove
		);
		camera.Move(fMove);
	}
	std::pair<float, float> rotVect = {
		float(wnd.mouse.GetPosX() - prevCameraPos.first) / 800.f * 180.f,
		float(wnd.mouse.GetPosY() - prevCameraPos.second) / 600.f * 180.f
	};
	//if (rotVect)
	if (rotVect.first > 90.f || rotVect.second > 90.f) return;
	camera.Rotate(XMFLOAT3(1.f, 0.f, 0.f), rotVect.first);
	camera.Rotate(XMFLOAT3(0.f, -1.f, 0.f), rotVect.second);
	prevCameraPos = wnd.mouse.GetPos();
}

void App::updateCamera() {
	auto cView = camera.View();
	wnd.Gfx().setCamera(
		DirectX::XMLoadFloat4x4(&cView)
	);
}