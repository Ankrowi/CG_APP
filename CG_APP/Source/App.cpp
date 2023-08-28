#include "App.h"
#include "Box.h"
#include <memory>
#include <algorithm>

constexpr float PI = 3.14159265f;

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
		std::uniform_int_distribution<int> longdist{ 10,40 };
		std::uniform_int_distribution<int> typedist{ 0,2 };
	};

	Factory f(wnd.Gfx());
	drawables.reserve(nDrawables);
	std::generate_n(std::back_inserter(drawables), nDrawables, f);

	wnd.Gfx().setProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
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
	for (auto& d : drawables) {
		d->Update(dt);
		d->Draw(wnd.Gfx());
	}
	wnd.Gfx().endFrame();
	std::string timerElapsed(std::to_string(1 / dt));
	wnd.setTitle(timerElapsed);
}