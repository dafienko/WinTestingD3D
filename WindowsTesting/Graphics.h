#pragma once

#include "lightWin.h"
#include <d3d11.h>
#include <wrl.h>
#include "ErrorHandler.h"
#include "Timer.h"

#define CHECK_HRESULT(hr)  CheckHRError(hr, __LINE__, __FILE__); if (hr < 0) throw std::exception()

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

class Graphics {
public:
	friend class Bindable;

	Graphics() = default;
	Graphics(HWND hWnd);
	~Graphics() = default;

	void ClearBuffer(float r, float g, float b);
	void Draw();
	void EndFrame();

	Microsoft::WRL::ComPtr<ID3D11Device> getPDevice();
	Microsoft::WRL::ComPtr <ID3D11DeviceContext> getPDeviceContext();

	Timer timer;
private:
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget = nullptr;
};