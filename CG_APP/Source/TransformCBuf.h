#pragma once
#include "ConstantBuffer.h"
#include "Drawable.h"
#include <DirectXMath.h>

class TransformCBuf : public Bindable
{
public:
		TransformCBuf(Graphics& gfx, const Drawable& parent);
		void bind(Graphics& gfx) override;
private:
	static std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> pVcbuf;
	const Drawable& parent;
};

