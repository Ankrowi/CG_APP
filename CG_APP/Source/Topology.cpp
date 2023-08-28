#include "Topology.h"

Topology::Topology(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type)
	: type(type)
{
}

void Topology::bind(Graphics& gfx)
{
	getContext(gfx)->IASetPrimitiveTopology(type);
}
