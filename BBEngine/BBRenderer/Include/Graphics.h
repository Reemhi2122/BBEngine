#pragma once
#include "BBWin.h"
#include "BBException.h"
#include <d3d11.h>
#include <wrl.h>
#include <DirectXMath.h>

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "Camera.h"

class Graphics {
public:
	ID3D11DeviceContext* GetContext() const noexcept;
	ID3D11Device* GetDevice() const noexcept;

	Graphics(HWND a_HWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();

	void EndFrame();
	void ClearBuffer(float a_Red, float a_Green, float a_Blue) noexcept;
	void DrawTestTriangle(float a_Angle, float x, float y, struct Vertex* ver, unsigned short* indices);
	void DrawIndexed(UINT a_Count) noexcept;

	void SetProjection(DirectX::XMMATRIX a_Projections);
	DirectX::XMMATRIX GetProjection() const noexcept;
	Camera* GetCamera();

private:
	DirectX::XMMATRIX m_Projection;
	Camera m_Camera;

	Microsoft::WRL::ComPtr<ID3D11Device>			m_Device;
	Microsoft::WRL::ComPtr <IDXGISwapChain>			m_SwapChain;
	Microsoft::WRL::ComPtr <ID3D11DeviceContext>	m_Context;
	Microsoft::WRL::ComPtr <ID3D11RenderTargetView> m_Target;
	Microsoft::WRL::ComPtr <ID3D11DepthStencilView> m_DepthStencilView;
};

inline ID3D11DeviceContext* Graphics::GetContext() const noexcept {
	return m_Context.Get();
}

inline ID3D11Device* Graphics::GetDevice() const noexcept {
	return m_Device.Get();
}

inline Camera* Graphics::GetCamera()
{
	return &m_Camera;
}