#include "Graphics.h"

#include "combaseapi.h"
#include <iostream>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#define SAFE_RELEASE(p) { if ( (p) ) { (p)->Release(); (p) = 0; } }

#define GET_CONSTANT_BUFFER_OFFSET_TYPE(p) ((sizeof(p) + 255) & (~255))
#define GET_CONSTANT_BUFFER_OFFSET_SIZE(p) ((p + 255) & (~255))

Graphics::Graphics(HWND a_HWnd)
 : m_HWindow(a_HWnd)
{
}

bool Graphics::Initialize()
{
	HRESULT hres;
	bool res = true;
	
	if (!CreateDevice())
	{
		printf("[GFX]: Failed to create the GFX device!");
		return false;
	}

	if (!CreateCommandQueue())
	{
		printf("[GFX]: Failed to create Command Queue!");
		return false;
	}

	if (!CreateDescriptorHeaps())
	{
		printf("[GFX]: Failed to create Descriptor Heaps!");
		return false;
	}

	if (!CreateSwapChain())
	{
		printf("[GFX]: Failed to create Swap Chain!");
		return false;
	}

	//Create the RTV Descriptor Heap
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeap = {};
	rtvDescriptorHeap.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescriptorHeap.NumDescriptors = TOTAL_RENDER_TARGETS;
	rtvDescriptorHeap.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvDescriptorHeap.NodeMask = 0;

	hres = m_Device->CreateDescriptorHeap(&rtvDescriptorHeap, IID_PPV_ARGS(&m_RTVDescriptorHeap));
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

	for(uint32_t i = 0; i < FRAME_TEXTURE_TARGET; i++)
	{
		hres = m_Device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM, WINDOW_WIDTH, WINDOW_HEIGHT, 1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			nullptr,
			IID_PPV_ARGS(&m_GameViewTarget[i])
		);
		if (FAILED(hres))
		{
			printf("[GFX]: Failed to get RTV for game view!");
			return false;
		}

		m_Device->CreateRenderTargetView(m_GameViewTarget[i], nullptr, rtvHandle);
		rtvHandle.Offset(1, m_RTVDescriptorSize);

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDescriptorDesc = {};
		srvDescriptorDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDescriptorDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDescriptorDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDescriptorDesc.Texture2D.MipLevels = 1;

		m_MainDescriptorFreeList.Alloc(&m_GRTVShaderResourceView[i].cpuDescHandle, &m_GRTVShaderResourceView[i].gpuDescHandle);
		m_Device->CreateShaderResourceView(m_GameViewTarget[i], &srvDescriptorDesc, m_GRTVShaderResourceView[i].cpuDescHandle);
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

	if(!CreateRootSignatures())
	{
		printf("[GFX]: Failed to create root signatures!");
		return false;
	}

	if(!CreateShaders())
	{
		printf("[GFX]: Failed to create shaders!");
		return false;
	}
	
	if(!CreateAllGraphicsContext())
	{
		printf("[GFX]: Failed to create PSOs!");
		return false;
	}

	D3D12_DESCRIPTOR_HEAP_DESC dsvDesc = {};
	dsvDesc.NumDescriptors = 1;
	dsvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	
	hres = m_Device->CreateDescriptorHeap(&dsvDesc, IID_PPV_ARGS(&m_DSDescriptorHeap));
	if (FAILED(hres))
	{
		printf("[GFX]: Failed to create DSV Descriptor Heap!");
		return false;
	}

	D3D12_DEPTH_STENCIL_VIEW_DESC dsViewDescription = {};
	dsViewDescription.Format		= DXGI_FORMAT_D32_FLOAT;
	dsViewDescription.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsViewDescription.Flags			= D3D12_DSV_FLAG_NONE;

	D3D12_CLEAR_VALUE dsClearValue = {};
	dsClearValue.Format = DXGI_FORMAT_D32_FLOAT;
	dsClearValue.DepthStencil.Depth = 1.0f;
	dsClearValue.DepthStencil.Stencil = 0.0f;

	hres = m_Device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, WINDOW_WIDTH, WINDOW_HEIGHT, 1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL),
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&dsClearValue,
		IID_PPV_ARGS(&m_DepthStenil)
	);
	m_DepthStenil->SetName(L"Depth Stencil Buffer");
	if (FAILED(hres))
	{
		printf("[GFX]: Failed create Depth Stencil Buffer heap descriptor!");
		return false;
	}
	
	m_Device->CreateDepthStencilView(m_DepthStenil, &dsViewDescription, m_DSDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

	//Creating a debug texture
	{
		D3D12_RESOURCE_DESC textureDescription = {};
		textureDescription.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		textureDescription.Alignment = 0;
		textureDescription.Width = 1600;
		textureDescription.Height = 900;
		textureDescription.DepthOrArraySize = 1;
		textureDescription.MipLevels = 1;
		textureDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDescription.SampleDesc.Count = 1;
		textureDescription.SampleDesc.Quality = 0;
		textureDescription.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		textureDescription.Flags = D3D12_RESOURCE_FLAG_NONE;

		hres = m_Device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE,
			&textureDescription,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(&m_DebugTexture)
		);
		if (FAILED(hres))
		{
			printf("[GFX::DX12:TEXTURE]: Failed to create Texture Default Heap!");
			return false;
		}
		m_DebugTexture->SetName(L"Debug Texture Heap");

		ID3D12Resource* debugTextureUploadHeap;
		hres = m_Device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(4),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&debugTextureUploadHeap)
		);
		if (FAILED(hres))
		{
			printf("[GFX::DX12:TEXTURE]: Failed to create Texture Upload Heap!");
			return false;
		}
		debugTextureUploadHeap->SetName(L"Texture Upload Heap");

		uint32_t debugTexture[1]{ 0xFF00FF };

		D3D12_SUBRESOURCE_DATA textureData = {};
		textureData.pData = debugTexture;
		textureData.RowPitch = 4;
		textureData.SlicePitch = 4;

		UpdateSubresources(m_CommandList, m_DebugTexture, debugTextureUploadHeap, 0, 0, 1, &textureData);
		m_CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_DebugTexture, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
	}

	//for (uint32_t i = 0; i < MAX_TEXTURES; i++)
	//{
	//	D3D12_SHADER_RESOURCE_VIEW_DESC srvDescriptorDesc = {};
	//	srvDescriptorDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//	srvDescriptorDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	//	srvDescriptorDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//	srvDescriptorDesc.Texture2D.MipLevels = 1;

	//	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuHandle;
	//	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle;
	//	m_MainDescriptorFreeList.Alloc(&cpuHandle, &gpuHandle);
	//	m_Device->CreateShaderResourceView(m_DebugTexture, &srvDescriptorDesc, cpuHandle);
	//}

	m_Viewport.TopLeftX = 0;
	m_Viewport.TopLeftY = 0;
	m_Viewport.Width = WINDOW_WIDTH; // Check window size
	m_Viewport.Height = WINDOW_HEIGHT; // Check window size
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;

	m_ScissorRect.top = 0;
	m_ScissorRect.left = 0;
	m_ScissorRect.right = WINDOW_WIDTH;
	m_ScissorRect.bottom = WINDOW_HEIGHT;

	m_Camera = new Camera();

	res = InitImGui();
	if (!res)
	{
		printf("[GFX]: Failed to initialize ImGui!");
		return false;
	}

	return true;
}

bool Graphics::InitImGui()
{
	ImGui_ImplDX12_InitInfo imguiInitInfo;
	imguiInitInfo.Device = m_Device;
	imguiInitInfo.CommandQueue = m_CommandQueue;
	imguiInitInfo.NumFramesInFlight = FRAME_BUFFER_COUNT;
	imguiInitInfo.RTVFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	imguiInitInfo.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	imguiInitInfo.UserData = reinterpret_cast<void*>(&m_MainDescriptorFreeList);
	imguiInitInfo.SrvDescriptorHeap = m_MainDescriptorFreeList.GetHeap(); //Note(Stan): Create a SRV descriptor heap for ImGui

	imguiInitInfo.SrvDescriptorAllocFn =
		[](ImGui_ImplDX12_InitInfo* info, D3D12_CPU_DESCRIPTOR_HANDLE* a_CPUHandle, D3D12_GPU_DESCRIPTOR_HANDLE* a_GPUHandle)
		{
			DescriptorFreeList* descriptorFreeList = reinterpret_cast<DescriptorFreeList*>(info->UserData);
			descriptorFreeList->Alloc(a_CPUHandle, a_GPUHandle);
		};

	imguiInitInfo.SrvDescriptorFreeFn =
		[](ImGui_ImplDX12_InitInfo* info, D3D12_CPU_DESCRIPTOR_HANDLE a_CPUHandle, D3D12_GPU_DESCRIPTOR_HANDLE a_GPUHandle)
		{
			reinterpret_cast<DescriptorFreeList*>(info->UserData)->Free(a_CPUHandle, a_GPUHandle);
		};

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(m_HWindow);
	ImGui_ImplDX12_Init(&imguiInitInfo);

	return true;
}

bool Graphics::CreateDevice()
{
	HRESULT hres = S_OK;
	IDXGIFactory4* dxgiFactory = nullptr;
	hres = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));
	if (FAILED(hres))
	{
		return false;
	}

	IDXGIAdapter1* dxgiAdapter = nullptr;
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

	hres = D3D12CreateDevice(dxgiAdapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_Device));
	if (FAILED(hres))
	{
		printf("[GFX]: Failed to create GFX Device with min level 11!");
		return false;
	}

	SAFE_RELEASE(dxgiAdapter);
	SAFE_RELEASE(dxgiFactory);

	return true;
}

bool Graphics::CreateSwapChain()
{
	HRESULT hres = S_OK;

	IDXGIFactory4* dxgiFactory = nullptr;
	hres = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));
	if (FAILED(hres))
	{
		return false;
	}

	DXGI_MODE_DESC backBufferDesc = {};
	backBufferDesc.Width = WINDOW_WIDTH; // Width of the window
	backBufferDesc.Height = WINDOW_HEIGHT; // Height of the window
	backBufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	DXGI_SAMPLE_DESC sampleDesc = {};
	sampleDesc.Count = 1;
	sampleDesc.Quality = 0;

	DXGI_SWAP_CHAIN_DESC  swapChainDesc = {};
	swapChainDesc.BufferDesc = backBufferDesc;
	swapChainDesc.SampleDesc = sampleDesc;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 3;
	swapChainDesc.OutputWindow = m_HWindow;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Flags = 0;

	IDXGISwapChain* tempSwapChain = nullptr;
	hres = dxgiFactory->CreateSwapChain(m_CommandQueue, &swapChainDesc, &tempSwapChain);
	if (FAILED(hres))
	{
		printf("[GFX]: Failed to create Swap Chain!");
		return false;
	}

	m_SwapChain = static_cast<IDXGISwapChain3*>(tempSwapChain);
	m_FrameIndex = m_SwapChain->GetCurrentBackBufferIndex();

	SAFE_RELEASE(dxgiFactory);

	return true;
}

bool Graphics::CreateDescriptorHeaps()
{
	bool res = S_OK;
	D3D12_DESCRIPTOR_HEAP_DESC srvDescriptorHeapDesc = {};
	srvDescriptorHeapDesc.NumDescriptors = MAX_TEXTURES;
	srvDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	res = m_MainDescriptorFreeList.Create(m_Device, &srvDescriptorHeapDesc);
	if (!res)
	{
		printf("[GFX]: Failed to Create SRV Resource Heap!");
		return false;
	}

	return true;
}

bool Graphics::CreateCommandQueue()
{
	HRESULT hres;
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc = {};
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

	return true;
}

bool Graphics::CreateRootSignatures()
{
	HRESULT hres = 0;
	D3D12_DESCRIPTOR_RANGE descriptorTableRange[1];
	descriptorTableRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorTableRange[0].NumDescriptors = MAX_TEXTURES;
	descriptorTableRange[0].BaseShaderRegister = 0;
	descriptorTableRange[0].RegisterSpace = 0;
	descriptorTableRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_ROOT_DESCRIPTOR_TABLE cbDescriptorTable = {};
	cbDescriptorTable.NumDescriptorRanges = 1;
	cbDescriptorTable.pDescriptorRanges = descriptorTableRange;

	D3D12_ROOT_DESCRIPTOR rootCBVDescriptor;
	rootCBVDescriptor.ShaderRegister = 0;
	rootCBVDescriptor.RegisterSpace = 0;

	D3D12_ROOT_PARAMETER rootParams[2] = {};
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParams[0].Descriptor = rootCBVDescriptor;
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[1].DescriptorTable = cbDescriptorTable;
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	D3D12_STATIC_SAMPLER_DESC staticSamplers[1];
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].MipLODBias = 0;
	staticSamplers[0].MaxAnisotropy = 0;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	staticSamplers[0].BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
	staticSamplers[0].MinLOD = 0;
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;
	staticSamplers[0].ShaderRegister = 0;
	staticSamplers[0].RegisterSpace = 0;
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
	rootSignatureDesc.NumParameters = 2;
	rootSignatureDesc.pParameters = rootParams;
	rootSignatureDesc.NumStaticSamplers = 1;
	rootSignatureDesc.pStaticSamplers = staticSamplers;
	rootSignatureDesc.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

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

	return true;
}

bool Graphics::CreateShaders()
{
	HRESULT hres = 0;
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

	m_VertexShaderByteCode = {};
	m_VertexShaderByteCode.BytecodeLength = m_VertexShader->GetBufferSize();
	m_VertexShaderByteCode.pShaderBytecode = m_VertexShader->GetBufferPointer();

	hres = D3DCompileFromFile(
		L"Assets/DefaultPS.hlsl", nullptr, nullptr,
		"main", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0, &m_PixelShader, &errorBuf);
	if (FAILED(hres))
	{
		printf("[GFX]: Failed to compile Pixel Shader with error code %s", (char*)errorBuf->GetBufferPointer());
		return false;
	}

	m_PixelShaderByteCode = {};
	m_PixelShaderByteCode.BytecodeLength = m_PixelShader->GetBufferSize();
	m_PixelShaderByteCode.pShaderBytecode = m_PixelShader->GetBufferPointer();

	hres = D3DCompileFromFile(
		L"Assets/VSCubeMap.hlsl", nullptr, nullptr,
		"main", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0, &m_CubeMapVertexShader, &errorBuf);
	if (FAILED(hres))
	{
		printf("[GFX]: Failed to compile Vertex Shader with error code %s", (char*)errorBuf->GetBufferPointer());
		return false;
	}

	m_CubeMapVertexShaderByteCode = {};
	m_CubeMapVertexShaderByteCode.BytecodeLength = m_CubeMapVertexShader->GetBufferSize();
	m_CubeMapVertexShaderByteCode.pShaderBytecode = m_CubeMapVertexShader->GetBufferPointer();

	hres = D3DCompileFromFile(
		L"Assets/PSCubeMap.hlsl", nullptr, nullptr,
		"main", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0, &m_CubeMapPixelShader, &errorBuf);
	if (FAILED(hres))
	{
		printf("[GFX]: Failed to compile Pixel Shader with error code %s", (char*)errorBuf->GetBufferPointer());
		return false;
	}

	m_CubeMapPixelShaderByteCode = {};
	m_CubeMapPixelShaderByteCode.BytecodeLength = m_CubeMapPixelShader->GetBufferSize();
	m_CubeMapPixelShaderByteCode.pShaderBytecode = m_CubeMapPixelShader->GetBufferPointer();

	return true;
}

bool Graphics::CreateAllGraphicsContext()
{
	HRESULT hres = 0;

	//////////////////////
	// **** PSO 01 **** //
	//////////////////////
	D3D12_INPUT_ELEMENT_DESC PSO1InputLayout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA , 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA , 0},
		{ "Normal",	 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	D3D12_INPUT_LAYOUT_DESC PSO1InputLayoutDesc = {};
	PSO1InputLayoutDesc.NumElements = sizeof(PSO1InputLayout) / sizeof(D3D12_INPUT_ELEMENT_DESC);
	PSO1InputLayoutDesc.pInputElementDescs = PSO1InputLayout;

	DXGI_SAMPLE_DESC PSO1SampleDesc = {};
	PSO1SampleDesc.Count = 1;
	PSO1SampleDesc.Quality = 0;

	//Note(Stan): Standard STENCIL_OP for now
	const D3D12_DEPTH_STENCILOP_DESC PSO1DefaultStencilOp = { D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_COMPARISON_FUNC_ALWAYS };

	D3D12_DEPTH_STENCIL_DESC PSO1DepthStencilDesc = {};
	PSO1DepthStencilDesc.DepthEnable = true;
	PSO1DepthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	PSO1DepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	PSO1DepthStencilDesc.StencilEnable = false;
	PSO1DepthStencilDesc.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
	PSO1DepthStencilDesc.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
	PSO1DepthStencilDesc.FrontFace = PSO1DefaultStencilOp;
	PSO1DepthStencilDesc.BackFace = PSO1DefaultStencilOp;

	//Create the Pipeline State Object
	D3D12_GRAPHICS_PIPELINE_STATE_DESC PSO1Desc = {};
	PSO1Desc.InputLayout = PSO1InputLayoutDesc;
	PSO1Desc.pRootSignature = m_RootSignature;
	PSO1Desc.VS = m_VertexShaderByteCode;
	PSO1Desc.PS = m_PixelShaderByteCode;
	PSO1Desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	PSO1Desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	PSO1Desc.SampleDesc = PSO1SampleDesc;
	PSO1Desc.SampleMask = 0xffffffff;
	PSO1Desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	PSO1Desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	PSO1Desc.NumRenderTargets = 1;
	PSO1Desc.DepthStencilState = PSO1DepthStencilDesc;

	hres = m_Device->CreateGraphicsPipelineState(&PSO1Desc, IID_PPV_ARGS(&m_PSOArray[0]));
	if (FAILED(hres))
	{
		printf("[GFX]: Failed to create the Graphics Pipeline Object");
		return false;
	}
	m_RenderContextMap["main"] = m_PSOArray[0];

	//////////////////////
	// **** PSO 02 **** //
	//////////////////////
	D3D12_INPUT_ELEMENT_DESC PSO2InputLayout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA , 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA , 0},
		{ "Normal",	 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	D3D12_INPUT_LAYOUT_DESC PSO2InputLayoutDescs = {};
	PSO2InputLayoutDescs.NumElements = sizeof(PSO2InputLayout) / sizeof(D3D12_INPUT_ELEMENT_DESC);
	PSO2InputLayoutDescs.pInputElementDescs = PSO2InputLayout;

	DXGI_SAMPLE_DESC PSO2sampleDesc = {};
	PSO2sampleDesc.Count = 1;
	PSO2sampleDesc.Quality = 0;

	//Note(Stan): Standard STENCIL_OP for now
	const D3D12_DEPTH_STENCILOP_DESC PSO2DefaultStencilOp = { D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_COMPARISON_FUNC_ALWAYS };

	D3D12_DEPTH_STENCIL_DESC PSO2DepthStencilDesc = {};
	PSO2DepthStencilDesc.DepthEnable = true;
	PSO2DepthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	PSO2DepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	PSO2DepthStencilDesc.StencilEnable = false;
	PSO2DepthStencilDesc.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
	PSO2DepthStencilDesc.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
	PSO2DepthStencilDesc.FrontFace = PSO2DefaultStencilOp;
	PSO2DepthStencilDesc.BackFace = PSO2DefaultStencilOp;

	D3D12_RASTERIZER_DESC rastirizerDesc = {};
	rastirizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	rastirizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	rastirizerDesc.FrontCounterClockwise = FALSE;
	rastirizerDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	rastirizerDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	rastirizerDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	rastirizerDesc.DepthClipEnable = TRUE;
	rastirizerDesc.MultisampleEnable = FALSE;
	rastirizerDesc.AntialiasedLineEnable = FALSE;
	rastirizerDesc.ForcedSampleCount = 0;
	rastirizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	//Create the Pipeline State Object
	D3D12_GRAPHICS_PIPELINE_STATE_DESC PSO2Desc = {};
	PSO2Desc.InputLayout = PSO2InputLayoutDescs;
	PSO2Desc.pRootSignature = m_RootSignature;
	PSO2Desc.VS = m_CubeMapVertexShaderByteCode;
	PSO2Desc.PS = m_CubeMapPixelShaderByteCode;
	PSO2Desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	PSO2Desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	PSO2Desc.SampleDesc = PSO2sampleDesc;
	PSO2Desc.SampleMask = 0xffffffff;
	PSO2Desc.RasterizerState = rastirizerDesc;
	PSO2Desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	PSO2Desc.NumRenderTargets = 1;
	PSO2Desc.DepthStencilState = PSO2DepthStencilDesc;

	hres = m_Device->CreateGraphicsPipelineState(&PSO2Desc, IID_PPV_ARGS(&m_PSOArray[1]));
	if (FAILED(hres))
	{
		printf("[GFX]: Failed to create the Graphics Pipeline Object");
		return false;
	}
	m_RenderContextMap["cubeMap"] = m_PSOArray[1];

	return true;
}

bool Graphics::SetGraphicsContext(const char* a_Context)
{
	m_CurPSO = m_RenderContextMap[a_Context];
	m_CommandList->SetPipelineState(m_CurPSO);
	return true;
}

bool Graphics::CloseInit()
{
	HRESULT hres;

	m_CommandList->Close();
	ID3D12CommandList* commandLists[] = { m_CommandList };
	m_CommandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);

	m_FenceValue[m_FrameIndex]++;
	hres = m_CommandQueue->Signal(m_Fence[m_FrameIndex], m_FenceValue[m_FrameIndex]);
	if (FAILED(hres))
	{
		printf("[GFX]: Failed to signal the Command Queue Fence!");
		return false;
	}

	return true;
}

void Graphics::Update()
{
	//Note(Stan): Currently not updating anything
}

void Graphics::StartFrame()
{
	HRESULT hres;

	WaitForPreviousFrame();

	hres = m_CommandAllocator[m_FrameIndex]->Reset();
	if (FAILED(hres))
	{
		printf("[GFX]: Failed reset the Command Allocator!");
		//TODO(Stan): Make a good way to cancel the update / graphics pipeline		
	}

	//Puts command list into recording state
	hres = m_CommandList->Reset(m_CommandAllocator[m_FrameIndex], m_CurPSO);
	if (FAILED(hres))
	{
		printf("[GFX]: Failed reset the Command list!");
		//TODO(Stan): Make a good way to cancel the update / graphics pipeline		
	}

	m_CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_GameViewTarget[m_FrameIndex], D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET));

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_RTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), 3 + m_FrameIndex, m_RTVDescriptorSize);
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsHandle(m_DSDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

	m_CommandList->OMSetRenderTargets(1, &rtvHandle, false, &dsHandle);

	FLOAT color[4]{ 0.32f, 0.40f, 0.45, 1.0f };
	m_CommandList->ClearRenderTargetView(rtvHandle, color, 0, nullptr);
	m_CommandList->ClearDepthStencilView(dsHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	m_CommandList->SetGraphicsRootSignature(m_RootSignature);

	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void Graphics::Render()
{
	HRESULT hres;
	ID3D12DescriptorHeap* descriptorHeaps[] = { m_MainDescriptorFreeList.GetHeap() };
	m_CommandList->SetDescriptorHeaps(1, descriptorHeaps);

	m_CommandList->SetGraphicsRootDescriptorTable(1, m_MainDescriptorFreeList.GetGPUHandleStart());

	m_CommandList->RSSetViewports(1, &m_Viewport);
	m_CommandList->RSSetScissorRects(1, &m_ScissorRect);
	m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Graphics::SetSwapBuffer()
{
	m_CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_RenderTargets[m_FrameIndex], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_RTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), m_FrameIndex, m_RTVDescriptorSize);
	m_CommandList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);

	FLOAT color[4]{ 0.32f, 0.40f, 0.45, 1.0f };
	m_CommandList->ClearRenderTargetView(rtvHandle, color, 0, nullptr);

	m_CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_GameViewTarget[m_FrameIndex], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
}

void Graphics::EndFrame()
{
	HRESULT hres;

	ImGui::Render();
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), m_CommandList);

	m_CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_RenderTargets[m_FrameIndex], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	hres = m_CommandList->Close();
	if (FAILED(hres))
	{
		printf("[GFX]: Failed to close the Command List!");
		//TODO(Stan): Make a good way to cancel the update / graphics pipeline		
	}

	ID3D12CommandList* cmdLists[] = {m_CommandList};
	m_CommandQueue->ExecuteCommandLists(1, cmdLists);

	hres = m_CommandQueue->Signal(m_Fence[m_FrameIndex], m_FenceValue[m_FrameIndex]);
	if (FAILED(hres))
	{
		printf("[GFX]: Failed to signal the Command Queue!");
		//TODO(Stan): Make a good way to cancel the update / graphics pipeline		
	}

	hres = m_SwapChain->Present(VSYNC, 0u);
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

void Graphics::DrawIndexed(uint32_t a_IndexCount)
{
	m_CommandList->DrawIndexedInstanced(a_IndexCount, 1, 0, 0, 0);
}

void Graphics::ShutDown()
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
		//SAFE_RELEASE(m_ConstantBufferUploadHeaps[i])
	}

	//SAFE_RELEASE(m_PSOArray[0]);
	//SAFE_RELEASE(m_PSOArray[1]);

	SAFE_RELEASE(m_RootSignature);
	//SAFE_RELEASE(m_VertexBuffer);
	//SAFE_RELEASE(m_IndexBuffer);

	//SAFE_RELEASE(m_DepthStenil);
	//SAFE_RELEASE(m_DSDescriptorHeap);
}

bool Graphics::GetRootConstantUploadBufferView(uint32_t a_RootParamIndex, uint32_t a_SizeOfCB, ConstantUploadBufferReference& a_ConstBufferReference)
{
	HRESULT hres;

	if (a_RootParamIndex > m_MaxRootCBV)
	{
		printf("[GFX::GetRootConstantUploadBufferView]: No root CBV upload heap at [%d] available!", a_RootParamIndex);
		return false;
	}

	if (a_SizeOfCB <= 0)
	{
		printf("[GFX::GetRootConstantUploadBufferView]: Cannot create / get root CBV upload buffer for CB with size of 0!");
		return false;
	}

	UploadHeap* curRootCBV = m_RootCBV[a_RootParamIndex];

	if (curRootCBV == nullptr)
	{
		curRootCBV = new UploadHeap();
		for (uint32_t i = 0; i < FRAME_BUFFER_COUNT; i++)
		{
			hres = m_Device->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(m_StandardCBSize),
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&curRootCBV->uploadHeaps[i])
			);
			curRootCBV->uploadHeaps[i]->SetName(L"Root sig CB Upload Resource Heap");
			curRootCBV->curOffset = 0;
			curRootCBV->size = m_StandardCBSize;


			CD3DX12_RANGE readRange(0, 0);
			hres = curRootCBV->uploadHeaps[i]->Map(0, &readRange, reinterpret_cast<void**>(&curRootCBV->cbvGPUAdress[i]));
		}

		m_RootCBV[a_RootParamIndex] = curRootCBV;
	}

	if ((curRootCBV->curOffset + a_SizeOfCB) > curRootCBV->size)
	{
		printf("[GFX::GetRootConstantUploadBufferView]: CBV heap at index [%d] is full!", a_RootParamIndex);
		return false;
	}

	a_ConstBufferReference.uploadHeapOffset = curRootCBV->curOffset;
	for (uint32_t i = 0; i < FRAME_BUFFER_COUNT; i++)
	{
		a_ConstBufferReference.cbvGPUAdress[i] = (curRootCBV->cbvGPUAdress[i] + curRootCBV->curOffset);
		a_ConstBufferReference.uploadHeaps[i] = curRootCBV->uploadHeaps[i];
	}
	
	curRootCBV->curOffset += GET_CONSTANT_BUFFER_OFFSET_SIZE(a_SizeOfCB);
	return true;
}