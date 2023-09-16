#pragma once


#include "IndexedTriangleList.h"
#include "DirectXMath.h"
#include "d3d11.h"

class SphereGeometry
{
public:
    template <class V>
    static IndexedTriangleList<V> Make(float radius, int sliceCount, int stackCount)
    {
        namespace dx = DirectX;

        std::vector<dx::XMFLOAT3> vertices;
        std::vector<unsigned short> indices;
        vertices.emplace_back(dx::XMFLOAT3(0, radius, 0));
        float phiStep = dx::XM_PI / stackCount;
        float thetaStep = 2.0f * dx::XM_PI / sliceCount;

        for (int i = 1; i <= stackCount - 1; i++) {
            auto phi = i * phiStep;
            for (int j = 0; j <= sliceCount; j++) {
                auto theta = j * thetaStep;
                auto p =  dx::XMFLOAT3(
                    (radius * sin(phi) * cos(theta)),
                    (radius * cos(phi)),
                    (radius * sin(phi) * sin(theta))
                );
                vertices.emplace_back(p);
            }
        }
        vertices.emplace_back(dx::XMFLOAT3(0, -radius, 0));

        for (int i = 1; i <= sliceCount; i++) {
            indices.emplace_back(0);
            indices.emplace_back(i + 1);
            indices.emplace_back(i);
        }
        int baseIndex = 1;
        int ringVertexCount = sliceCount + 1;
        for (int i = 0; i < stackCount - 2; i++) {
            for (int j = 0; j < sliceCount; j++) {
                indices.emplace_back(baseIndex + i * ringVertexCount + j);
                indices.emplace_back(baseIndex + i * ringVertexCount + j + 1);
                indices.emplace_back(baseIndex + (i + 1) * ringVertexCount + j);

                indices.emplace_back(baseIndex + (i + 1) * ringVertexCount + j);
                indices.emplace_back(baseIndex + i * ringVertexCount + j + 1);
                indices.emplace_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
            }
        }
        int southPoleIndex = vertices.size() - 1;
        baseIndex = southPoleIndex - ringVertexCount;
        for (int i = 0; i < sliceCount; i++) {
            indices.emplace_back(southPoleIndex);
            indices.emplace_back(baseIndex + i);
            indices.emplace_back(baseIndex + i + 1);
        }

        std::vector<V> verts(vertices.size());
        for (size_t i = 0; i < vertices.size(); i++)
        {
            verts[i].pos = vertices[i];
        }
        return {
            std::move(verts),
            std::move(indices)
        };
    }
};