#pragma once
#include "BBWin.h"

#include <d3d12.h>
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

constexpr uint8_t FRAME_BUFFER_COUNT = 3;

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
	
	//Fence values
	ID3D12Fence*	m_Fence[FRAME_BUFFER_COUNT];
	HANDLE			m_FenceEvent;
	uint64_t		m_FenceValue[FRAME_BUFFER_COUNT];

	uint32_t m_FrameIndex;
	uint32_t m_RTVDescriptorSize;

};