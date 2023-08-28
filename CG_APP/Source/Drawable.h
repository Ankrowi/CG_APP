#pragma once

#include "Graphics.h"
#include <DirectXMath.h>
class Bindable;

class Drawable
{
	template<class T>
	friend class DrawableBase;
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	virtual DirectX::XMMATRIX GetTransformXM() const = 0;
	void Draw(Graphics& gfx) const;
	virtual void Update(float db) = 0;
	void AddBind(std::unique_ptr<Bindable> bind);
	void AddIndexBuffer(std::unique_ptr<class IndexBuffer> iBuf);
	virtual ~Drawable() = default;
private:
	virtual const std::vector<std::unique_ptr<Bindable>>& getStaticBinds() const = 0;
	const class IndexBuffer* pIndexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> binds;
};

