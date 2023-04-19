#pragma once
#include "BBWin.h"
#include "BBException.h"
#include <d3d11.h>
#include <dxerr.h>
#include <wrl.h>

class Graphics {
public:
	class Exception : public BBException {
		using BBException::BBException;
	};

	class HrException : public Exception {
	public:
		HrException(int a_Line, const char* a_File, HRESULT a_Hr) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
		std::string GetErrorDescription() const noexcept;

	private:
		HRESULT m_Hr;
	};

	class DeviceRemovedException : public HrException {
		using HrException::HrException;

	public:
		const char* GetType() const noexcept override;
	};

	Graphics(HWND a_HWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics() = default;

	void EndFrame();
	void ClearBuffer(float a_Red, float a_Green, float a_Blue) noexcept;

private:
	Microsoft::WRL::ComPtr<ID3D11Device>			m_Device;
	Microsoft::WRL::ComPtr <IDXGISwapChain>			m_SwapChain;
	Microsoft::WRL::ComPtr <ID3D11DeviceContext>	m_Context;
	Microsoft::WRL::ComPtr <ID3D11RenderTargetView> m_Target;
};