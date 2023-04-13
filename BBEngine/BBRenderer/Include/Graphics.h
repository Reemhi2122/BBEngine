#pragma once
#include "../BBWin.h"
#include <d3d11.h>

class Graphics {
public:
	Graphics(HWND a_HWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();

	void EndFrame();
	void ClearBuffer(float a_Red, float a_Green, float a_Blue) noexcept 
	{
		const float color[] = { a_Red, a_Green, a_Blue, 1.0f };
		m_Context->ClearRenderTargetView(m_Target, color);
	}

private:
	ID3D11Device* m_Device = nullptr;
	IDXGISwapChain* m_SwapChain = nullptr;
	ID3D11DeviceContext* m_Context = nullptr;
	ID3D11RenderTargetView* m_Target = nullptr;
};