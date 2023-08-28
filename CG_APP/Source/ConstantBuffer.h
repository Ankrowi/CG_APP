#pragma once
#include "Bindable.h"

template<typename C>
class ConstantBuffer : public Bindable
{
public:
	ConstantBuffer(Graphics& gfx, const C& consts) {
		D3D11_BUFFER_DESC cbd;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(consts);
		cbd.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA csd = {};
		csd.pSysMem = &consts;
		getDevice(gfx)->CreateBuffer(&cbd, &csd, &pConstantBuffer);
	}
	ConstantBuffer(Graphics& gfx) {
		D3D11_BUFFER_DESC cbd;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(C);
		cbd.StructureByteStride = 0u;
		getDevice(gfx)->CreateBuffer(&cbd, nullptr, &pConstantBuffer);
	}
	void Update(Graphics& gfx, const C& consts) {
		D3D11_MAPPED_SUBRESOURCE msr;
		getContext(gfx)->Map(pConstantBuffer.Get(), 0u,
			D3D11_MAP_WRITE_DISCARD, 0u, &msr);
		memcpy(msr.pData, &consts, sizeof(consts));
		getContext(gfx)->Unmap(pConstantBuffer.Get(), 0u);
	}
protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
};

template<typename C>
class VertexConstantBuffer: public ConstantBuffer<C>
{
	using ConstantBuffer<C>::pConstantBuffer;
	using Bindable::getContext;
public:
	using ConstantBuffer<C>::ConstantBuffer;
	void bind(Graphics& gfx) {
		getContext(gfx)->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());
	}
};

template<typename C>
class PixelConstantBuffer : public ConstantBuffer<C> {
	using ConstantBuffer<C>::pConstantBuffer;
	using Bindable::getContext;
public:
	using ConstantBuffer<C>::ConstantBuffer;
	void bind(Graphics& gfx) {
		getContext(gfx)->PSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());
	}
};

