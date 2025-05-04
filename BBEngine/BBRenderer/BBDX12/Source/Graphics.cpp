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
	descriptorHeapDesc.NumDescriptors = 3;
	descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descriptorHeapDesc.NodeMask = 0;

	hres = m_Device->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&m_RTVDescriptorHeap));
	if (FAILED(hres))
	{
		printf("[GFX]: Failed to create RTV descriptor heap!");
		return false;
	}

	return true;
}

void Graphics::Cleanup()
{

}