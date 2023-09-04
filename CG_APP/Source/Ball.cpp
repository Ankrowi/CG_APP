#include "Ball.h"
#include "BindableBase.h"
#include "Circle.h"
#include <iostream>

Ball::Ball(Graphics& gfx, float x, float y) :
	moveSpeed(0.0f), angle(0.0f), position({x, y})
{
	namespace dx = DirectX;

	if (!isStaticInitialized())
	{
		struct Vertex
		{
			dx::XMFLOAT2 pos;
		};
		auto const model = Circle::Make<Vertex>();
		addStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

		auto pvs = std::make_unique<VertexShader>(gfx, L"./Shaders/PongVS.cso");
		auto pvsbc = pvs->getBytecode();
		addStaticBind(std::move(pvs));

		addStaticBind(std::make_unique<PixelShader>(gfx, L"./Shaders/PongPS.cso"));

		addStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "POSITION",0,DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 }
		};
		addStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

		addStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		setIndexFromStatic();
	}

	AddBind(std::make_unique<TransformCBuf>(gfx, *this));

	dx::XMStoreFloat3x3(
		&mt,
		dx::XMMatrixScaling(0.07f, 0.07f, 1.0f)
	);
}

DirectX::XMMATRIX Ball::GetTransformXM() const
{
	namespace dx = DirectX;
	return dx::XMLoadFloat3x3(&mt) *
		dx::XMMatrixTranslation(position.x, position.y, 1.0f);
}

void Ball::Update(float db)
{
	namespace dx = DirectX;
	position.x += db * moveSpeed * cos(angle * DirectX::XM_PI / 180);
	position.y += db * moveSpeed * sin(angle * DirectX::XM_PI / 180);
}

void Ball::setSpeed(float speed)
{
	moveSpeed = speed;
}

void Ball::setAngle(float angle) {
	this->angle = angle;
}

std::pair<float, float> Ball::getLocation() {
	return std::pair<float, float>(position.x, position.y);
}
