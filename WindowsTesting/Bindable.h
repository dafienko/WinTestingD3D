#pragma once

#include "Graphics.h"

class Bindable {
public:
	virtual void Bind(Graphics* gfx) = 0;
	virtual ~Bindable() = default;
protected:
	static Microsoft::WRL::ComPtr<ID3D11Device> getPDevice(Graphics* gfx);
	static Microsoft::WRL::ComPtr <ID3D11DeviceContext> getPDeviceContext(Graphics* gfx);
};