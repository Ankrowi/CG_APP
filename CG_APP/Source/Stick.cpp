#include "Stick.h"
#include "BindableBase.h"
#include "Rectangle.h"
#include <iostream>

Stick::Stick(Graphics& gfx, float x, float y) :
	moveSpeed(0.0f), motion(0.0f), position({ x, y })
{
	namespace dx = DirectX;

	if (!isStaticInitialized())
	{
		struct Vertex
		{ 
			dx::XMFLOAT2 pos;
		};
		auto const model = Rectangle::Make<Vertex>();
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
		dx::XMMatrixScaling(0.06f, 0.3f, 1.0f)
	);

}

DirectX::XMMATRIX Stick::GetTransformXM() const
{
	namespace dx = DirectX;
    return dx::XMLoadFloat3x3(&mt) * 
		dx::XMMatrixTranslation(position.posX, position.posY, 1.0f);
}

void Stick::Update(float db)
{
	namespace dx = DirectX;
	position.posY -= db * motion * moveSpeed;
}

void Stick::setInpit(float motion)
{
    this->motion = motion;
}

void Stick::setSpeed(float speed)
{
    moveSpeed = speed;
}

std::pair<float, float> Stick::getLocation()
{
	return std::pair<float, float>(position.posX, position.posY);
}

float Stick::getInput() 
{
	return motion;
}