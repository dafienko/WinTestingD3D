#include "Bindable.h"

Microsoft::WRL::ComPtr<ID3D11Device> Bindable::getPDevice(Graphics* gfx) {
	return gfx->getPDevice();
}

Microsoft::WRL::ComPtr <ID3D11DeviceContext> Bindable::getPDeviceContext(Graphics* gfx) {
	return gfx->getPDeviceContext();
}