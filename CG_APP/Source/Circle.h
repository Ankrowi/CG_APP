#pragma once

#include "IndexedTriangleList.h"
#include <DirectXMath.h>
#include <cmath>

class Circle
{
public:
	template<class V>
	static IndexedTriangleList<V> Make()
	{
		namespace dx = DirectX;

		constexpr float R = 1.0f / 2.0f;
		constexpr float alpha = 30.0f * dx::XM_PI / 180.0f;
		constexpr int num = 3;
		float angle = alpha;

		std::vector<dx::XMFLOAT2> vertices;
		std::vector<unsigned short> indices;
		
		vertices.emplace_back(dx::XMFLOAT2(0.0f, 0.0f));
		vertices.emplace_back(dx::XMFLOAT2(0.0f, R));
		int id = 2;
		//top left region
		for (int i = 0; i < num; i++) {
			auto& last = vertices.at(vertices.size() - 1);
			vertices.emplace_back(dx::XMFLOAT2(- R * sin(angle),
				R * cos(angle)));
			addIndices(indices, id++);
			angle += alpha;
		}
		//bottom left region
		for (int i = 0; i < num; i++) {
			auto& last = vertices.at(vertices.size() - 1);
			vertices.emplace_back(dx::XMFLOAT2(- R * sin(angle),
				R * cos(angle)));
			addIndices(indices, id++);
			angle += alpha;
		}
		//bottom right region
		for (int i = 0; i < num; i++) {
			auto& last = vertices.at(vertices.size() - 1);
			vertices.emplace_back(dx::XMFLOAT2(-R * sin(angle),
				R * cos(angle)));
			addIndices(indices, id++);
			angle += alpha;
		}
		//top right region
		for (int i = 0; i < num; i++) {
			auto& last = vertices.at(vertices.size() - 1);
			vertices.emplace_back(dx::XMFLOAT2( - R * sin(angle),
				R * cos(angle)));
			addIndices(indices, id++);
			angle += alpha;
		}
		std::vector<V> verts(vertices.size());
		for (size_t i = 0; i < vertices.size(); i++)
		{
			verts[i].pos = vertices[i];
		}
		return{
			std::move(verts), std::move(indices)
		};
	}
private:
	static void addIndices(std::vector<unsigned short>& vect, int idx) {
		vect.emplace_back(0);
		vect.emplace_back(idx);
		vect.emplace_back(idx - 1);
		
	}
};