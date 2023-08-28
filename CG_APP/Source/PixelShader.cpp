#include "PixelShader.h"

PixelShader::PixelShader(Graphics& gfx, const std::wstring& path)
{
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	D3DReadFileToBlob(path.c_str(), &pBlob);
	getDevice(gfx)->CreatePixelShader(pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(), nullptr, &pPixelShader);
}

void PixelShader::bind(Graphics& gfx)
{
	getContext(gfx)->PSSetShader(pPixelShader.Get(), nullptr, 0u);
}
