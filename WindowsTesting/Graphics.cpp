#include "Graphics.h"
#include <wrl.h>
#include <d3dcompiler.h>
#include <stdlib.h>
#include <array>

namespace wrl = Microsoft::WRL;

Graphics::Graphics(HWND hWnd) : timer{ Timer() } {
	
	DXGI_SWAP_CHAIN_DESC sd = { 0 };
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	CHECK_HRESULT(D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwapChain,
		&pDevice,
		NULL,
		&pContext
	));

	ID3D11Resource* pBackBuffer;
	
	CHECK_HRESULT(pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), (void**)&pBackBuffer));
	
	CHECK_HRESULT(pDevice->CreateRenderTargetView(
		pBackBuffer,
		NULL,
		&pTarget
	));

	pBackBuffer->Release();
}


void Graphics::EndFrame() {
	pSwapChain->Present(0u, 0u);
}

void Graphics::ClearBuffer(float r, float g, float b) {
	const float color[] = { r, g, b, 1.0f };
	pContext->ClearRenderTargetView(pTarget.Get(), color);
}

void Graphics::Draw() {
	struct Vertex {
		float x, y, z, r, g, b;
	};

	const Vertex vertices[] = {
		{0, .5f, 0,			1, 0, 0},
		{0.5f, -.5f, 0,		0, 1, 0},
		{-.5f, -.5f, 0,		0, 0, 1}
	};

	D3D11_BUFFER_DESC pDesc = { 0 };
	pDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	pDesc.Usage = D3D11_USAGE_DEFAULT;
	pDesc.CPUAccessFlags = NULL;
	pDesc.MiscFlags = NULL;
	pDesc.ByteWidth = sizeof(vertices);
	pDesc.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA sd = { 0 };
	sd.pSysMem = vertices;

	wrl::ComPtr<ID3D11Buffer> pVertexBuffer;

	CHECK_HRESULT(pDevice->CreateBuffer(&pDesc, &sd, &pVertexBuffer));

	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	pContext->IASetVertexBuffers( 0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset );


	wrl::ComPtr<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"Color", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(float) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	wrl::ComPtr<ID3DBlob> pBlob;

	wrl::ComPtr<ID3D11PixelShader> pPixelShader;
	D3DReadFileToBlob(L"PixelShader.cso", &pBlob);
	CHECK_HRESULT(pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &pPixelShader));

	wrl::ComPtr<ID3D11VertexShader> pVertexShader;
	D3DReadFileToBlob(L"VertexShader.cso", &pBlob);
	CHECK_HRESULT(pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &pVertexShader));
	
	CHECK_HRESULT(pDevice->CreateInputLayout(
		ied,
		(UINT)std::size(ied),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pInputLayout
	));

	pContext->IASetInputLayout(pInputLayout.Get());

	pContext->VSSetShader(pVertexShader.Get(), 0, 0);
	pContext->PSSetShader(pPixelShader.Get(), 0, 0);


	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);

	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D11_VIEWPORT VP;
	VP.Width = 400;
	VP.Height = 400;
	VP.MinDepth = 0;
	VP.MaxDepth = 1;
	VP.TopLeftX = 0;
	VP.TopLeftY = 0;
	pContext->RSSetViewports(1u, &VP);

	pContext->Draw( (UINT)std::size(vertices), 0u);
}

Microsoft::WRL::ComPtr<ID3D11Device> Graphics::getPDevice() {
	return pDevice;
}

Microsoft::WRL::ComPtr<ID3D11DeviceContext> Graphics::getPDeviceContext() {
	return pContext;
}