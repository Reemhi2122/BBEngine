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
#include "Materials.h"

#include "IGraphics.h"

//Note(Stan): Switch these out with custom containers
#include <vector>
#include <unordered_map>

constexpr uint16_t WINDOW_WIDTH = 1600;
constexpr uint16_t WINDOW_HEIGHT = 900;

struct Model;

namespace BBE
{
	enum class AlphaMode;
}

struct VertexShader
{
	Microsoft::WRL::ComPtr<ID3D11VertexShader>	m_Shader;
	Microsoft::WRL::ComPtr<ID3DBlob> 			m_ByteCodeBlob;
	std::string									m_Path;
};

struct PixelShader
{
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	m_Shader;
	std::string									m_Path;
};

class DX11InputLayout;
class DX11Sampler;
class DX11Topology;

struct GraphicsContext
{
	BBHandle VertexShader;
	BBHandle PixelShader;
	DX11InputLayout* Layout;
	DX11Sampler* Sampler;
	DX11Topology* Topology;

	~GraphicsContext()
	{
		delete Layout;
		Layout = nullptr;
		delete Sampler;
		Sampler = nullptr;
		delete Topology;
		Topology = nullptr;
	}
};

class Graphics : public IGraphics {
public:
	ID3D11DeviceContext* GetContext() const noexcept override;
	ID3D11Device* GetDevice() const noexcept override;

	Graphics(HWND a_HWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();

	bool Initialize() override;
	bool CloseInit() override { return true; };

	void Update() override {};

	void StartFrame() override;
	void Render() override {};
	void EndFrame() override;

	void SetSwapBuffer() {};

	void WaitForPreviousFrame() override {};
	void ShutDown() override {};

	void DrawTestTriangle(float a_Angle, float x, float y, struct Vertex* ver, unsigned short* indices);
	void DrawIndexed(UINT a_Count) noexcept;
	void DrawIndexedInstanced(UINT a_IndexCount, UINT a_InstanceCount) noexcept;
	
	Camera* GetCamera() const override;
	void SetCamera(Camera* a_Camera) override;

	void ResetRenderTarget();
	void SetGameViewRenderTarget();

	void SetDepthStencilTarget();
	void SetDepthStencilTarget(ID3D11DepthStencilView* a_Target);
	
	void BindDepthSampler();

	void BindDepthTexture(ID3D11ShaderResourceView* a_View, uint32_t a_StartSlot, uint32_t a_NumViews);
	void BindSampler(ID3D11SamplerState* a_Sampler, uint32_t a_StartSlot, uint32_t a_NumViews);

	void SetBlendState(BBE::AlphaMode a_BlendMode);

	void UnbindSRV(uint32_t a_Slot);

	// == Lights functions ==
	void CreateDirLightShadowBuffer();

	void CreatePointLightDepthCubeMapArray();
	void CreatePointLightDepthCubeMap(ID3D11DepthStencilView** a_DepthStencilArray, uint32_t index);
	
	void CreateSpotLightDepthMapArray();
	void CreateSpotLightDepthTexture(ID3D11DepthStencilView** a_DepthStencilArray, uint32_t index);

	//uint32_t CreateTexture(const char* a_Path);

	ID3D11ShaderResourceView* GetDirectionLightDepthMapRSV()	{ return m_DirLightDepthBufferSRV; };
	ID3D11ShaderResourceView* GetPointLightDepthCubeArrayRSV()	{ return m_PointLightDepthCubeArraySRV; };
	ID3D11ShaderResourceView* GetSpotLightDepthMapArrayRSV()	{ return m_SpotLightsDepthArraySRV; };
	ID3D11ShaderResourceView* GetGameViewRSV()					{ return m_GameWindowSRV; };

	BBHandle CreateShader(ShaderType a_Type, std::string a_Path, std::string a_EntryPointFunc = "main");
	void BindShader(ShaderType a_Type, BBHandle a_Shader);
	void ReloadShader(ShaderType a_Type, BBHandle a_Shader);
	
	//Note(Stan): Testing out context switching (PSO for DX12)
	//Note(Stan): Switch these out with custom containers
	GraphicsContext m_AllRenderContext[5];
	std::unordered_map<std::string, GraphicsContext*> m_RenderContextMap;
	bool CreateAllGraphicsContext() override;
	bool SetGraphicsContext(const char* a_Context) override;

	VertexShader* GetVertexShaderArray() noexcept { return (VertexShader*)m_VertexShaders; }
	PixelShader* GetPixelShaderArray() noexcept { return (PixelShader*)m_PixelShaders; }

	Microsoft::WRL::ComPtr<ID3DBlob> GetVertexShaderByteCode(BBHandle a_Shader) const;

	ID3D11ShaderResourceView* m_TextureDepthSRV[6];
	ID3D11ShaderResourceView* m_SpotLightsDepthTest = nullptr;

private:
	HWND	m_HWindow;
	Camera* m_Camera;

	Microsoft::WRL::ComPtr<ID3D11Device>			m_Device;
	Microsoft::WRL::ComPtr<IDXGISwapChain>			m_SwapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		m_Context;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	m_Target;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	m_DepthStencilView;

	ID3D11Texture2D*			m_GameWindowRT = nullptr;
	ID3D11RenderTargetView*		m_GameWindowRTV = nullptr;
	ID3D11ShaderResourceView*   m_GameWindowSRV = nullptr;
	
	ID3D11Texture2D*			m_TextureCubeMap = nullptr;
	ID3D11SamplerState*			m_DepthTextureSampler = nullptr;

	// Directional lights
	ID3D11Texture2D*			m_DirLightDepthBuffer = nullptr;
	ID3D11DepthStencilView*		m_DirLightTextureDSV = nullptr;
	ID3D11ShaderResourceView*	m_DirLightDepthBufferSRV = nullptr;

	// Spot lights
	ID3D11Texture2D* m_SpotLightsDepthArray = nullptr;
	ID3D11DepthStencilView* m_SpotLightTextureDSV[TOTAL_SPOTLIGHTS];
	ID3D11ShaderResourceView* m_SpotLightsDepthArraySRV = nullptr;

	// Point lights
	ID3D11Texture2D*			m_PointLightDepthCubeArray = nullptr;
	ID3D11DepthStencilView*		m_PointLightTextureDSV[TOTAL_POINTLIGHTS][6];
	ID3D11ShaderResourceView*	m_PointLightDepthCubeArraySRV = nullptr;

	ID3D11BlendState*			m_TransparancyBlendState;
	
	uint32_t			m_VertexIndex = 0u;
	VertexShader		m_VertexShaders[100u];
	uint32_t			m_PixelIndex = 0u;
	PixelShader			m_PixelShaders[100u];
};

inline ID3D11DeviceContext* Graphics::GetContext() const noexcept {
	return m_Context.Get();
}

inline ID3D11Device* Graphics::GetDevice() const noexcept {
	return m_Device.Get();
}

inline Camera* Graphics::GetCamera() const
{
	return m_Camera;
}

inline void Graphics::SetCamera(Camera* a_Camera) {

	m_Camera = a_Camera;
	m_Context->RSSetViewports(1, a_Camera->GetViewPort());
}