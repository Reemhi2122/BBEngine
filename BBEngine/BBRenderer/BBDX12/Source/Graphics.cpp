#include "Graphics.h"

#include "combaseapi.h"
#include <iostream>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

Graphics::Graphics(HWND a_HWnd)
 : m_HWindow(a_HWnd)
{

}

bool Graphics::Initialize()
{
	HRESULT hres;

	IDXGIFactory4* dxgiFactory;
	hres = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));
	if (FAILED(hres))
	{
		return false;
	}

	IDXGIAdapter1* dxgiAdapter;
	uint32_t adapterIndex = 0;
	bool adapterFound = false;

	while (dxgiFactory->EnumAdapters1(adapterIndex, &dxgiAdapter) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_ADAPTER_DESC1 desc;
		dxgiAdapter->GetDesc1(&desc);
		if(!(desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE))
		{
			hres = D3D12CreateDevice(dxgiAdapter, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr);
			if (SUCCEEDED(hres))
			{
				adapterFound = true;
				break;
			}
		}

		adapterIndex++;
	}

	if (!adapterFound)
	{
		printf("[GFX]: No DX12 compatible device found!");
		return false;
	}


	//Create the graphics device
	hres = D3D12CreateDevice(dxgiAdapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_Device));
	//Prob want to move this into a macro
	if (FAILED(hres))
	{
		printf("[GFX]: Failed to create GFX Device with min level 11!");
		return false;
	}

	//Create the Command Queue
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	commandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	commandQueueDesc.NodeMask = 0;

	hres = m_Device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&m_CommandQueue));
	if (FAILED(hres))
	{
		printf("[GFX]: Failed to create direct Command Queue!");
		return false;
	}

	//Create the Back Buffer
	DXGI_MODE_DESC backBufferDesc{};
	backBufferDesc.Width = 1600; // Width of the window
	backBufferDesc.Height = 900; // Height of the window
	backBufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	
	DXGI_SAMPLE_DESC sampleDesc{};
	sampleDesc.Count = 1;
	sampleDesc.Quality = 0;

	DXGI_SWAP_CHAIN_DESC  swapChainDesc{};
	swapChainDesc.BufferDesc = backBufferDesc;
	swapChainDesc.SampleDesc = sampleDesc;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 3;
	swapChainDesc.OutputWindow = m_HWindow;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Flags = 0;

	IDXGISwapChain* tempSwapChain;
	hres = dxgiFactory->CreateSwapChain(m_Device, &swapChainDesc, &tempSwapChain);
	if (FAILED(hres))
	{
		printf("[GFX]: Failed to create Swap Chain!");
		return false;
	}

	m_SwapChain = static_cast<IDXGISwapChain3*>(tempSwapChain);
	m_FrameIndex = m_SwapChain->GetCurrentBackBufferIndex();

	//Create the RTV Descriptor Heap
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc{};
	descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	descriptorHeapDesc.NumDescriptors = FRAME_BUFFER_COUNT;
	descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	descriptorHeapDesc.NodeMask = 0;

	hres = m_Device->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&m_RTVDescriptorHeap));
	if (FAILED(hres))
	{
		printf("[GFX]: Failed to create RTV descriptor heap!");
		return false;
	}

	m_RTVDescriptorSize = m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_RTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
	
	for (uint32_t i = 0; i < FRAME_BUFFER_COUNT; i++)
	{
		hres = m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&m_RenderTargets[i]));
		if (FAILED(hres))
		{
			printf("[GFX]: Failed to get RTV from Swap Chain!");
			return false;
		}

		m_Device->CreateRenderTargetView(m_RenderTargets[i], nullptr, rtvHandle);
		rtvHandle.Offset(1, m_RTVDescriptorSize);
	}

	//Create the Command Buffer Allocators
	for (uint32_t i = 0; i < FRAME_BUFFER_COUNT; i++)
	{
		hres = m_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_CommandAllocator[i]));
		if (FAILED(hres))
		{
			printf("[GFX]: Failed to create Command Allocator!");
			return false;
		}
	}

	//Create the Command List
	hres = m_Device->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		m_CommandAllocator[0],
		NULL,
		IID_PPV_ARGS(&m_CommandList)
	);
	if (FAILED(hres))
	{
		printf("[GFX]: Failed to create Command List!");
		return false;
	}

	m_CommandList->Close();

	//Create fence values
	for (uint32_t i = 0; i < FRAME_BUFFER_COUNT; i++)
	{
		hres = m_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence[i]));
		if (FAILED(hres))
		{
			printf("[GFX]: Failed to create Fences!");
			return false;
		}
		m_FenceValue[i] = 0;
	}

	//Create the actual fence event
	m_FenceEvent = CreateEvent(nullptr, false, false, nullptr);
	if (!m_FenceEvent)
	{
		printf("[GFX]: Failed to create Fence Event!");
		return false;
	}

	return true;
}

void Graphics::Update()
{
	// Nothing yet
}

void Graphics::UpdatePipeline()
{
	HRESULT hres;

	// Still have to implement the fence check
	WaitForPreviousFrame();

	hres = m_CommandAllocator[m_FrameIndex]->Reset();
	if (FAILED(hres))
	{
		printf("[GFX]: Failed reset the Command Allocator!");
		//TODO(Stan): Make a good way to cancel the update / graphics pipeline		
	}

	//Puts command list into recording state
	hres = m_CommandList->Reset(m_CommandAllocator[m_FrameIndex], nullptr);
	if (FAILED(hres))
	{
		printf("[GFX]: Failed reset the Command list!");
		//TODO(Stan): Make a good way to cancel the update / graphics pipeline		
	}

	m_CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_RenderTargets[m_FrameIndex], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
	
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_RTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), m_FrameIndex, m_RTVDescriptorSize);

	m_CommandList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);

	FLOAT color[4]{ 1.0f, 0.0f, 0.0f, 1.0f };
	m_CommandList->ClearRenderTargetView(rtvHandle, color, 0, nullptr);

	hres = m_CommandList->Close();
	if (FAILED(hres))
	{
		printf("[GFX]: Failed to close the Command List!");
		//TODO(Stan): Make a good way to cancel the update / graphics pipeline		
	}
}

void Graphics::Render()
{
	HRESULT hres;

	UpdatePipeline();

	ID3D12CommandList* cmdLists[] = {m_CommandList};

	m_CommandQueue->ExecuteCommandLists(1, cmdLists);
	
	hres = m_CommandQueue->Signal(m_Fence[m_FrameIndex], m_FenceValue[m_FrameIndex]);
	if (FAILED(hres))
	{
		printf("[GFX]: Failed to signal the Command Queue!");
		//TODO(Stan): Make a good way to cancel the update / graphics pipeline		
	}

	hres = m_SwapChain->Present(0, 0);
	if (FAILED(hres))
	{
		printf("[GFX]: Failed to present!");
		//TODO(Stan): Make a good way to cancel the update / graphics pipeline		
	}
}

void Graphics::WaitForPreviousFrame()
{
	HRESULT hres;

	m_FrameIndex = m_SwapChain->GetCurrentBackBufferIndex();

	if (m_Fence[m_FrameIndex]->GetCompletedValue() < m_FenceValue[m_FrameIndex])
	{
		hres = m_Fence[m_FrameIndex]->SetEventOnCompletion(m_FenceValue[m_FrameIndex], m_FenceEvent);
		if (FAILED(hres))
		{
			printf("[GFX]: Failed to set event on completion!");
			//TODO(Stan): Make a good way to cancel the update / graphics pipeline		
		}

		WaitForSingleObject(m_FenceEvent, INFINITE);
	}

	m_FenceValue[m_FrameIndex]++;
}

void Graphics::Cleanup()
{
	// Nothing yet
}