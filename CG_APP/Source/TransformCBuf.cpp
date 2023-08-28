#include "TransformCBuf.h"

TransformCBuf::TransformCBuf(Graphics& gfx, const Drawable& parent) 
	:
	parent(parent)
{
	if (!pVcbuf) {
		pVcbuf = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>(gfx);
	}
}

void TransformCBuf::bind(Graphics& gfx)
{
	pVcbuf->Update(gfx, DirectX::XMMatrixTranspose(
		parent.GetTransformXM() * gfx.getProjection()
	));
	pVcbuf->bind(gfx);
}

std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> TransformCBuf::pVcbuf;