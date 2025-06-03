#include "Graphics.h"

#include "combaseapi.h"
#include <iostream>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#define SAFE_RELEASE(p) { if ( (p) ) { (p)->Release(); (p) = 0; } }

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
		if (!(desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE))
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
	backBufferDesc.Width = WIND0W_WIDTH; // Width of the window
	backBufferDesc.Height = WIND0W_HEIGHT; // Height of the window
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
	hres = dxgiFactory->CreateSwapChain(m_CommandQueue, &swapChainDesc, &tempSwapChain);
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

	//Note(Stan): Most of this code down here is for a temporary triangle
	//Creating a Root Signature
	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ID3DBlob* signature;
	hres = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, nullptr);
	if (FAILED(hres))
	{
		printf("[GFX]: Failed to serialize Root Signature!");
		return false;
	}

	hres = m_Device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_RootSignature));
	if (FAILED(hres))
	{
		printf("[GFX]: Failed to create Root Signature!");
		return false;
	}

	//Compile Vertex Shader
	ID3DBlob* errorBuf;
	hres = D3DCompileFromFile(
		L"Assets/DefaultVS.hlsl", nullptr, nullptr,
		"main", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0, &m_VertexShader, &errorBuf);
	if (FAILED(hres))
	{
		printf("[GFX]: Failed to compile Vertex Shader with error code %s", (char*)errorBuf->GetBufferPointer());
		return false;
	}

	//Set the Vertex Shader Byte Code
	m_VertexShaderByteCode = {};
	m_VertexShaderByteCode.BytecodeLength = m_VertexShader->GetBufferSize();
	m_VertexShaderByteCode.pShaderBytecode = m_VertexShader->GetBufferPointer();

	//Compile the Vertex Shader
	hres = D3DCompileFromFile(
		L"Assets/DefaultPS.hlsl", nullptr, nullptr,
		"main", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0, &m_PixelShader, &errorBuf);
	if (FAILED(hres))
	{
		printf("[GFX]: Failed to compile Pixel Shader with error code %s", (char*)errorBuf->GetBufferPointer());
		return false;
	}

	m_PixelShaderByteCode = {};
	m_PixelShaderByteCode.BytecodeLength = m_PixelShader->GetBufferSize();
	m_PixelShaderByteCode.pShaderBytecode = m_PixelShader->GetBufferPointer();

	//Create the Input Layout
	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA , 0}
	};

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc = {};
	inputLayoutDesc.NumElements = sizeof(inputLayout) / sizeof(D3D12_INPUT_ELEMENT_DESC);
	inputLayoutDesc.pInputElementDescs = inputLayout;

	//Create the Pipeline State Object
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.InputLayout = inputLayoutDesc;
	psoDesc.pRootSignature = m_RootSignature;
	psoDesc.VS = m_VertexShaderByteCode;
	psoDesc.PS = m_PixelShaderByteCode;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.SampleDesc = sampleDesc;
	psoDesc.SampleMask = 0xffffffff;
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.NumRenderTargets = 1;

	hres = m_Device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_DefaultPipelineState));
	if (FAILED(hres))
	{
		printf("[GFX]: Failed to create the Graphics Pipeline Object");
		return false;
	}

	TempVertex vertexList[] =
	{
		{{+0.0f, +0.5f, +0.5f}},
		{{+0.5f, -0.5f, +0.5f}},
		{{-0.5f, -0.5f, +0.5f}}
	};

	int vertexBufferSize = sizeof(vertexList);
	
	m_Device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&m_VertexBuffer)
	);
	m_VertexBuffer->SetName(L"Vertex Buffer Default Heap");

	ID3D12Resource* vertexBufferUploadHeap;
	m_Device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertexBufferUploadHeap)
	);
	vertexBufferUploadHeap->SetName(L"Vertex Buffer Upload Heap");

	D3D12_SUBRESOURCE_DATA vertexData = {};
	vertexData.pData = reinterpret_cast<BYTE*>(vertexList);
	vertexData.RowPitch = vertexBufferSize;
	vertexData.SlicePitch = vertexBufferSize;

	UpdateSubresources(m_CommandList, m_VertexBuffer, vertexBufferUploadHeap, 0, 0, 1, &vertexData);

	m_CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_VertexBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));

	m_CommandList->Close();
	ID3D12CommandList* commandLists[] = { m_CommandList };
	m_CommandQueue->ExecuteCommandLists(1, commandLists);

	m_FenceValue[m_FrameIndex++];
	hres = m_CommandQueue->Signal(m_Fence[m_FrameIndex], m_FenceValue[m_FrameIndex]);
	if (FAILED(hres))
	{
		printf("[GFX]: Failed to signal the Command Queue Fence!");
		return false;
	}

	m_VertexBufferView.BufferLocation = m_VertexBuffer->GetGPUVirtualAddress();
	m_VertexBufferView.StrideInBytes = sizeof(TempVertex);
	m_VertexBufferView.SizeInBytes = vertexBufferSize;

	m_Viewport.TopLeftX = 0;
	m_Viewport.TopLeftY = 0;
	m_Viewport.Width = WIND0W_WIDTH; // Check window size
	m_Viewport.Height = WIND0W_HEIGHT; // Check window size
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;

	m_siccorRect.left = 0;
	m_siccorRect.top = 0;
	m_siccorRect.right = WIND0W_WIDTH;
	m_siccorRect.bottom = WIND0W_HEIGHT;

	return true;
}

void Graphics::Update()
{
	//Nothing yet
}

void Graphics::UpdatePipeline()
{
	HRESULT hres;

	//Still have to implement the fence check
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

	FLOAT color[4]{ 0.0f, 0.0f, 1.0f, 1.0f };
	m_CommandList->ClearRenderTargetView(rtvHandle, color, 0, nullptr);

	m_CommandList->SetGraphicsRootSignature(m_RootSignature);
	m_CommandList->RSSetViewports(1, &m_Viewport);
	m_CommandList->RSSetScissorRects(1, &m_siccorRect);
	m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_CommandList->IASetVertexBuffers(0, 1, &m_VertexBufferView);
	m_CommandList->DrawInstanced(3, 1, 0, 0);

	m_CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_RenderTargets[m_FrameIndex], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

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
	for (uint32_t i = 0; i < FRAME_BUFFER_COUNT; i++)
	{
		m_FrameIndex = i;
		WaitForPreviousFrame();
	}
	
	SAFE_RELEASE(m_Device);
	SAFE_RELEASE(m_SwapChain);
	SAFE_RELEASE(m_CommandQueue);
	SAFE_RELEASE(m_RTVDescriptorHeap);
	SAFE_RELEASE(m_CommandList);

	for (uint32_t i = 0; i < FRAME_BUFFER_COUNT; i++)
	{
		SAFE_RELEASE(m_RenderTargets[i]);
		SAFE_RELEASE(m_CommandAllocator[i]);
		SAFE_RELEASE(m_Fence[i]);
	}
}