#pragma once

#include "IndexedTriangleList.h"
#include <DirectXMath.h>

class Rectangle
{
public:
	template<class V>
	static IndexedTriangleList<V> Make()
	{
		namespace dx = DirectX;

		constexpr float side = 1.0f / 2.0f;

		std::vector<dx::XMFLOAT2> vertices;
		vertices.emplace_back(-side, -side); // 0
		vertices.emplace_back(side, -side); // 1
		vertices.emplace_back(-side, side); // 2
		vertices.emplace_back(side, side); // 3

		std::vector<V> verts(vertices.size());
		for (size_t i = 0; i < vertices.size(); i++)
		{
			verts[i].pos = vertices[i];
		}
		return{
			std::move(verts),{
				0,2,1 , 2,3,1
			}
		};
	}
};