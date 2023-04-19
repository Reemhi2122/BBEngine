#include "Graphics.h"
#include <sstream>

#pragma comment(lib, "d3d11.lib")

#define GFX_THROW_FAILED(hrcall) do{ if(FAILED(hr = (hrcall))) throw Graphics::HrException(__LINE__, __FILE__, hr); } while(0)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) do{ throw Graphics::DeviceRemovedException(__LINE__, __FILE__, (hr)); } while(0)

Graphics::Graphics(HWND a_HWnd)
{
	DXGI_SWAP_CHAIN_DESC sd = {};
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
	sd.OutputWindow = a_HWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	HRESULT hr;

	GFX_THROW_FAILED(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&m_SwapChain,
		&m_Device,
		nullptr,
		&m_Context));

	int isf = 0;
	isf = isf + 5;

	Microsoft::WRL::ComPtr<ID3D11Resource> backBuffer;
	GFX_THROW_FAILED(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer));
	GFX_THROW_FAILED(m_Device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_Target));
	backBuffer->Release();
}

void Graphics::EndFrame()
{
	HRESULT hr;
	if (FAILED(hr = m_SwapChain->Present(1u, 0u))) {
		if (hr == DXGI_ERROR_DEVICE_REMOVED) {
			GFX_DEVICE_REMOVED_EXCEPT(m_Device->GetDeviceRemovedReason());
		}
		else {
			GFX_THROW_FAILED(hr);
		}
	}
}

void Graphics::ClearBuffer(float a_Red, float a_Green, float a_Blue) noexcept
{
	const float color[] = { a_Red, a_Green, a_Blue, 1.0f };
	m_Context->ClearRenderTargetView(m_Target.Get(), color);
}

Graphics::HrException::HrException(int a_Line, const char* a_File, HRESULT a_Hr) noexcept
	:	Exception(a_Line, a_File),
		m_Hr (a_Hr)
{}

const char* Graphics::HrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Error String] " << GetErrorString() << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl;
	oss << GetOriginString();

	m_WhatBuffer = oss.str();
	return m_WhatBuffer.c_str();
}

const char* Graphics::HrException::GetType() const noexcept
{
	return "[BB Graphics Exception]";
}

HRESULT Graphics::HrException::GetErrorCode() const noexcept
{
	return m_Hr;
}

std::string Graphics::HrException::GetErrorString() const noexcept
{
	return DXGetErrorString(m_Hr);
}

std::string Graphics::HrException::GetErrorDescription() const noexcept
{
	char buf[512];
	DXGetErrorDescription(m_Hr, buf, sizeof(buf));
	return buf;
}

const char* Graphics::DeviceRemovedException::GetType() const noexcept
{
	return "[BB Graphics Exception] [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}