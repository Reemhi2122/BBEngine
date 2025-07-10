#pragma once
#include "BBWin.h"

#include <d3d12.h>
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>

#include "D3DX12/d3dx12.h"

#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"

#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include <queue>

#include "Camera.h"
#include "IGraphics.h"

#include "DescriptorFreeList.h"

//Temp testing bindables
#include "Bindable/DX12VertexBuffer.h"
#include "Bindable/DX12IndexBuffer.h"
#include "Bindable/DX12Texture.h"
#include "Bindable/DX12ConstantBuffer.h"

constexpr uint16_t WINDOW_WIDTH = 1600;
constexpr uint16_t WINDOW_HEIGHT = 800;

#define MAX_TEXTURES 1024

//Note(stan): Temp vertext buffer for test triangle
struct UV
{
	float u, v;
};

struct TempVertex
{
	Vector3 vertex;
	UV		texCoord;
};

struct ConstantBufferPerObject
{
	DirectX::XMFLOAT4X4 WVPMatrix;
	DirectX::XMFLOAT4X4 Transform;
};

struct SRVDescriptorInfo
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandle;
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandle;
};

struct UploadHeap
{
	ID3D12Resource* uploadHeaps[FRAME_BUFFER_COUNT];
	uint8_t* cbvGPUAdress[FRAME_BUFFER_COUNT];
	uint32_t curOffset;
	uint32_t size;
};

class Graphics : public IGraphics
{
public:
	Graphics(HWND a_HWnd);

	//Note(Stan): Some starter funtions gotten from DX12 tut
	bool Initialize() override;
	bool CloseInit() override;

	void Update() override;

	void StartFrame() override;
	void Render() override;
	void EndFrame() override;

	void ShutDown() override;
	void WaitForPreviousFrame() override;

	Camera* GetCamera() const override;
	void SetCamera(Camera* a_Camera) override;

	ID3D12Device* GetDevice() const override;
	
	ID3D12GraphicsCommandList* GetCommandList() const;

	uint8_t GetFrameCount() const override { return FRAME_BUFFER_COUNT; };
	uint8_t GetCurrentFrame() const override { return m_FrameIndex; };

	CD3DX12_CPU_DESCRIPTOR_HANDLE* GetCurrentViewHandle() { return &m_GRTVShaderResourceView[m_FrameIndex].cpuDescHandle; };
	void SetSwapBuffer();

	bool GetRootConstantUploadBufferView(uint32_t a_RootParamIndex, uint32_t a_SizeOfCB, struct ConstantUploadBufferReference& a_ConstBufferReference);

	DescriptorFreeList* GetMainDescriptorHeap() { return &m_MainDescriptorFreeList; };

	void DrawIndexed(uint32_t a_IndexCount) override;

private:
	HWND						m_HWindow;
	Camera*						m_Camera;

	ID3D12Device*				m_Device;
	IDXGISwapChain3*			m_SwapChain; //Tripple buffering swap chain
	ID3D12CommandQueue*			m_CommandQueue;
	ID3D12DescriptorHeap*		m_RTVDescriptorHeap;
	
	SRVDescriptorInfo			m_GRTVShaderResourceView[FRAME_BUFFER_COUNT];
	ID3D12Resource*				m_GameViewTarget[FRAME_BUFFER_COUNT];
	
	ID3D12Resource*				m_RenderTargets[FRAME_BUFFER_COUNT];

	ID3D12CommandAllocator*		m_CommandAllocator[FRAME_BUFFER_COUNT];
	ID3D12GraphicsCommandList*	m_CommandList;
	
	//Note(Stan): Used for rendering the first triangle, likely to change later
	ID3D12PipelineState*		m_DefaultPipelineState;
	ID3D12RootSignature*		m_RootSignature;
	D3D12_VIEWPORT				m_Viewport;
	D3D12_RECT					m_ScissorRect;

	//ID3D12DescriptorHeap*		m_MainDescriptorHeap;
	DescriptorFreeList			m_MainDescriptorFreeList;

	ID3D12Resource*				m_DepthStenil;
	ID3D12DescriptorHeap*		m_DSDescriptorHeap;

	ID3D12Resource*				m_DebugTexture;

	uint32_t m_StandardCBSize = (1024 * 64);
	uint32_t m_MaxRootCBV = 1;
	UploadHeap* m_RootCBV[1] = { nullptr };

	//Note(Stan): Temp, these will be replaced by actual objects
	DirectX::XMFLOAT4X4	m_Cube1WorldMatrix;
	DirectX::XMFLOAT4X4	m_Cube1RotationMatrix;
	DirectX::XMFLOAT4	m_Cube1Pos;

	DirectX::XMFLOAT4X4	m_Cube2WorldMatrix;
	DirectX::XMFLOAT4X4	m_Cube2RotationMatrix;
	DirectX::XMFLOAT4	m_Cube2PosOffset;

	uint32_t			m_NumOfCubeIndices;

	//Shaders
	ID3DBlob*	m_VertexShader;
	D3D12_SHADER_BYTECODE m_VertexShaderByteCode;

	ID3DBlob*	m_PixelShader;
	D3D12_SHADER_BYTECODE m_PixelShaderByteCode;

	//Fence values
	ID3D12Fence*	m_Fence[FRAME_BUFFER_COUNT];
	HANDLE			m_FenceEvent;
	uint64_t		m_FenceValue[FRAME_BUFFER_COUNT];

	uint32_t m_FrameIndex;
	uint32_t m_RTVDescriptorSize;

	//Temp testing bindables
	IVertexBuffer* m_CubeVertexBuffer;
	IIndexBuffer* m_CubeIndexBuffer;
	ITexture* m_Texture;

	SRVDescriptorInfo m_TextureDescriptors[MAX_TEXTURES];
	std::queue<SRVDescriptorInfo*> m_AvailableTextureDescriptors;

	IConstantBuffer<ConstantBufferPerObject>* m_ConstantBufferCube1;
	IConstantBuffer<ConstantBufferPerObject>* m_ConstantBufferCube2;
};

inline ID3D12Device* Graphics::GetDevice() const
{
	return m_Device;
}

inline ID3D12GraphicsCommandList* Graphics::GetCommandList() const
{
	return m_CommandList;
}

inline Camera* Graphics::GetCamera() const
{
	return m_Camera;
}

inline void Graphics::SetCamera(Camera* a_Camera)
{
	m_Camera = a_Camera;
}
