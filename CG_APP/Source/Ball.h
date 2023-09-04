#pragma once
#include "DrawableBase.h"
#include "DirectXCollision.h"
#include "CollisionRectangle.h"
#include "SimpleMath.h"

class Ball : public DrawableBase<Ball>
{
public:
	Ball(Graphics& gfx, float x, float y);
	void Update(float dt) override;
	DirectX::XMMATRIX GetTransformXM() const override;
	void setSpeed(float speed);
	void setAngle(float angle);
	std::pair<float, float> getLocation();
	DirectX::SimpleMath::Rectangle Rect;
private:
	float moveSpeed;
	float angle;
	struct {
		float x;
		float y;
	} position;
	DirectX::XMFLOAT3X3 mt;
	
};

