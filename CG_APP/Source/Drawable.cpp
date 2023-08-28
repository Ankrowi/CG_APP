#include "Drawable.h"
#include "IndexBuffer.h"

void Drawable::Draw(Graphics& gfx) const
{
	for (auto& b : binds) {
		b->bind(gfx);
	}
	for (auto& b : getStaticBinds()) {
		b->bind(gfx);
	}
	gfx.drawIndexed(pIndexBuffer->getCount());
}

void Drawable::AddBind(std::unique_ptr<Bindable> bind)
{
	binds.push_back(std::move(bind));
}

void Drawable::AddIndexBuffer(std::unique_ptr<class IndexBuffer> iBuf)
{
	pIndexBuffer = iBuf.get();
	binds.push_back(std::move(iBuf));
}
