#pragma once
#include "DrawableBase.h"
#include "CollisionRectangle.h"
#include <d3d11.h>
#include "SimpleMath.h"



class Stick : public DrawableBase<Stick>
{
public:
	Stick(Graphics& gfx, float x, float y);
	// Унаследовано через DrawableBase
	virtual DirectX::XMMATRIX GetTransformXM() const override;
	virtual void Update(float db) override;
	void setInpit(float motion);
	float getInput();
	void setSpeed(float speed);
	DirectX::SimpleMath::Rectangle Rect;
	std::pair<float, float> getLocation();
private:
	float motion;
	float moveSpeed;
	struct
	{
		float posX;
		float posY;
	} position;
	DirectX::XMFLOAT3X3 mt;
};

