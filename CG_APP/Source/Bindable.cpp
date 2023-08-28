#include "Bindable.h"
#include "Bindable.h"

ID3D11DeviceContext* Bindable::getContext(Graphics& gfx) noexcept {
	return gfx.pContext.Get();
}

ID3D11Device* Bindable::getDevice(Graphics& gfx) noexcept
{
	return gfx.pDevice.Get();
}
