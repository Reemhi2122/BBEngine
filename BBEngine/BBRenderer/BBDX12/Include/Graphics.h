#pragma once
#include "BBWin.h"

#include <d3d12.h>
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>

#include "D3DX12/d3dx12.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"

constexpr uint8_t FRAME_BUFFER_COUNT = 3;

constexpr uint16_t WINDOW_WIDTH = 1600;
constexpr uint16_t WINDOW_HEIGHT = 800;

//Note(stan): Temp vertext buffer for test triangle
struct TempVertex
{
	Vector3 vertex;
	Vector4 color;
};

struct ConstantBufferPerObject
{
	BBMath::Matrix4x4 colorMultiplier;
};

class Graphics
{
public:
	Graphics(HWND a_HWnd);

	//Note(Stan): Some starter funtions gotten from DX12 tut
	bool Initialize();
	void Update();
	void UpdatePipeline();
	void Render();
	void Cleanup();
	void WaitForPreviousFrame();

private:
	HWND						m_HWindow;
	
	ID3D12Device*				m_Device;
	IDXGISwapChain3*			m_SwapChain; //Tripple buffering swap chain
	ID3D12CommandQueue*			m_CommandQueue;
	ID3D12DescriptorHeap*		m_RTVDescriptorHeap;
	ID3D12Resource*				m_RenderTargets[FRAME_BUFFER_COUNT];
	ID3D12CommandAllocator*		m_CommandAllocator[FRAME_BUFFER_COUNT];
	ID3D12GraphicsCommandList*	m_CommandList;
	
	//Note(Stan): Used for rendering the first triangle, likely to change later
	ID3D12PipelineState*		m_DefaultPipelineState;
	ID3D12RootSignature*		m_RootSignature;
	D3D12_VIEWPORT				m_Viewport;
	D3D12_RECT					m_ScissorRect;

	ID3D12Resource*				m_VertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW	m_VertexBufferView;

	ID3D12Resource*				m_IndexBuffer;
	D3D12_INDEX_BUFFER_VIEW 	m_IndexBufferView;

	ID3D12Resource*				m_DepthStenil;
	ID3D12DescriptorHeap*		m_DSDescriptorHeap;

	ConstantBufferPerObject		m_CBPerObject;
	ID3D12Resource*				m_ConstantBufferUploadHeaps[FRAME_BUFFER_COUNT];
	uint8_t*					m_CBVGPUAdress[FRAME_BUFFER_COUNT];

	DirectX::XMFLOAT4X4	m_CameraProjMatrix;
	DirectX::XMFLOAT4X4	m_CameraViewMatrix;

	//Note(Stan): Temp, these will be replaced by the cam class
	DirectX::XMFLOAT4	m_CameraPos;
	DirectX::XMFLOAT4	m_CameraTarget;
	DirectX::XMFLOAT4	m_CameraUp;

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

};

//Note(Stan): Old constant buffer code
//Note(Stan): Temp constant buffer testing
//ID3D12DescriptorHeap*		m_CBDescriptorHeap[FRAME_BUFFER_COUNT];
//ID3D12Resource*				m_CBUploadHeap[FRAME_BUFFER_COUNT];
//
//CBColorMultiply				m_CBColorMultiplier;
//uint8_t*					m_CBColorMultiplierGPUAdress[FRAME_BUFFER_COUNT];