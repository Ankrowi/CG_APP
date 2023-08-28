#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <memory>
#include <vector>
#include <string>
#include <DirectXMath.h>
#pragma warning(disable:4265)
#include <wrl.h>
#pragma warning(default:4265)

class Graphics
{
	friend class Bindable;
public:
	Graphics(HWND hWnd);
	~Graphics();
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	void endFrame();
	void drawIndexed(UINT count) noexcept;
	void setProjection(DirectX::FXMMATRIX proj) noexcept;
	DirectX::XMMATRIX getProjection();
	void clearBuffer(float red, float green, float blue);
private:
	DirectX::XMMATRIX projection;
private:
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;
};

