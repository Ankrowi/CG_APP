#include "VertexShader.h"

VertexShader::VertexShader(Graphics& gfx, const std::wstring& path)
{
	D3DReadFileToBlob(path.c_str(), &pBytecodeBlob);
	getDevice(gfx)->CreateVertexShader(
		pBytecodeBlob->GetBufferPointer(),
		pBytecodeBlob->GetBufferSize(),
		nullptr, &pVertexShader);
}

void VertexShader::bind(Graphics& gfx)
{
	getContext(gfx)->VSSetShader(pVertexShader.Get(), nullptr, 0u);
}

ID3DBlob* VertexShader::getBytecode() const
{
	return pBytecodeBlob.Get();
}
