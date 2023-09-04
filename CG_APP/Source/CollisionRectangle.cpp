#include "CollisionRectangle.h"

CRectangle::CRectangle(DirectX::XMFLOAT2 centre, DirectX::XMFLOAT2 extents)
{
	points[0] = { centre.x, centre.y };
	points[1] = { centre.x + extents.x, centre.y + extents.y };
	points[2] = { centre.x - extents.x, centre.y + extents.y };
	points[3] = { centre.x - extents.x, centre.y - extents.y };
	points[4] = { centre.x + extents.x, centre.y - extents.y };
}

CRectangle::CRectangle(DirectX::XMFLOAT2 topLeft, DirectX::XMFLOAT2 topRight, DirectX::XMFLOAT2 bottomLeft, DirectX::XMFLOAT2 bottomRight)
{
	points[0] = { (topLeft.x + topRight.x) / 2,
		(topLeft.y + bottomLeft.y) / 2 };
	points[1] = point(topRight);
	points[2] = point(topLeft);
	points[3] = point(bottomLeft);
	points[4] = point(bottomRight);
}

CRectangle::CRectangle() {
	points[0] = points[1] = points[2] = points[3] = points[4] = point(0, 0);
}

bool CRectangle::Intersects(const CRectangle& other)
{
	auto& op = other.points;
	auto& mp = points;
	if (op[3].y >= mp[1].y) return false;
	if (op[1].y <= mp[3].y) return false;
	if (op[1].x <= mp[3].x) return false;
	if (op[3].x >= mp[1].x) return false;
	return true;
}

DirectX::XMFLOAT2 CRectangle::getCentre()
{
	return DirectX::XMFLOAT2(points[0].x, points[0].y);
}

DirectX::XMFLOAT2 CRectangle::getExtents()
{
	return DirectX::XMFLOAT2(abs(points[0].x - points[1].x), abs(points[0].y - points[1].y));
}

std::vector<DirectX::XMFLOAT2> CRectangle::getVertsAsVector()
{
	std::vector<DirectX::XMFLOAT2> verts;
	for (int i = 1; i < 5; ++i)
		verts.emplace_back(points[i].toXMFLOAT2());
	return std::move(verts);
}
