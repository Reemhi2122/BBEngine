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
#include "Vertexconstant.h"

#include <vector>

typedef uint32_t TMPHANDLE;
struct Model;

template<typename Type = cbPerFrame>
struct VertexConstantBuffer;

enum class ShaderType
{
	VertexShader,
	PixelShader,
	GeometryShader,
	ComputeShader,
	HullShader
	//TODO: Add all shader types, not really using something else then PS and VS right now
};

struct VertexShader
{
	Microsoft::WRL::ComPtr <ID3D11VertexShader>	m_VertexShader;
	Microsoft::WRL::ComPtr<ID3DBlob> 			m_ByteCodeBlob;
};

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
	void DrawIndexedInstanced(UINT a_IndexCount, UINT a_InstanceCount) noexcept;

	void SetProjection(DirectX::XMMATRIX a_Projections);
	DirectX::XMMATRIX GetProjection() const noexcept;
	
	Camera* GetCamera();
	void SetCamera(Camera* a_Camera);

	void ResetRenderTarget();
	void CalculateLightShadowMap(std::vector<Model*>& a_Models, uint32_t a_VSShadowMapShader, uint32_t a_PSShadowMapShader, DirectX::XMMATRIX spotLightMatrix, Camera& a_Cam1, Camera& a_Cam2);

	void SetGameViewRenderTarget();
	void SetDepthStencilTarget();
	
	void BindDepthTexture();
	void UnbindSRV(uint32_t a_Slot);

	TMPHANDLE CreateShader(ShaderType a_Type, std::wstring a_Path);
	void BindShader(ShaderType a_Type, TMPHANDLE a_Shader);
	Microsoft::WRL::ComPtr<ID3DBlob> GetVertexShaderByteCode(TMPHANDLE a_Shader) const;

private:
	DirectX::XMMATRIX m_Projection;
	Camera* m_Camera;

	Microsoft::WRL::ComPtr<ID3D11Device>			m_Device;
	Microsoft::WRL::ComPtr<IDXGISwapChain>			m_SwapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		m_Context;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	m_Target;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	m_DepthStencilView;

	ID3D11Texture2D*			m_TextureTarget = nullptr;
	ID3D11RenderTargetView*		m_TextureRenderTargetView = nullptr;
	ID3D11ShaderResourceView*	m_TextureShaderResourceView = nullptr;

	ID3D11Texture2D*			m_TextureDepthTarget = nullptr;
	ID3D11DepthStencilView*		m_TextureDepthStencilView = nullptr;
	ID3D11ShaderResourceView*	m_TextureDepthShaderResourceView = nullptr;

	ID3D11Texture2D*	m_TextureCubeMap = nullptr;
	ID3D11SamplerState* m_DepthTextureSampler = nullptr;

	uint32_t			m_VertexIndex = 0u;
	VertexShader		m_VertexShaders[100u];
	uint32_t			m_PixelIndex = 0u;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShaders[100u];
};

inline ID3D11DeviceContext* Graphics::GetContext() const noexcept {
	return m_Context.Get();
}

inline ID3D11Device* Graphics::GetDevice() const noexcept {
	return m_Device.Get();
}

inline Camera* Graphics::GetCamera()
{
	return m_Camera;
}

inline void Graphics::SetCamera(Camera* a_Camera) {
	m_Camera = a_Camera;
}