#include "InputLayout.h"

InputLayout::InputLayout(Graphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderBytecode)
{
	getDevice(gfx)->CreateInputLayout(layout.data(), (UINT)layout.size(),
		pVertexShaderBytecode->GetBufferPointer(),
		pVertexShaderBytecode->GetBufferSize(),
		&pInputLayout);
}

void InputLayout::bind(Graphics& gfx)
{
	getContext(gfx)->IASetInputLayout(pInputLayout.Get());
}
