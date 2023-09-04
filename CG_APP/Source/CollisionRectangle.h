#pragma once
#include "d3d11.h"
#include "DirectXMath.h"
#include <vector>

class CRectangle {
private:
	// 0 - centre, 1-4 - top:bottom, right:left (anti-clockwise)
	struct point {
		float x;
		float y;
		point(DirectX::XMFLOAT2 coord) {
			x = coord.x;
			y = coord.y;
		}
		point(float _x, float _y) {
			x = _x;
			y = _y;
		}
		point() {
			x = 0.f;
			y = 0.f;
		}
		DirectX::XMFLOAT2 toXMFLOAT2() {
			return DirectX::XMFLOAT2(x, y);
		}
	} points[5];
public:
	CRectangle(DirectX::XMFLOAT2 centre, DirectX::XMFLOAT2 extents);
	CRectangle(DirectX::XMFLOAT2 topLeft, DirectX::XMFLOAT2 topRight,
		DirectX::XMFLOAT2 bottomLeft, DirectX::XMFLOAT2 bottonRight);
	CRectangle();
	bool Intersects(const CRectangle& other);
	DirectX::XMFLOAT2 getCentre();
	DirectX::XMFLOAT2 getExtents();
	std::vector<DirectX::XMFLOAT2> getVertsAsVector();
};