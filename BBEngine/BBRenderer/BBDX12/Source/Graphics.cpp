#include "Graphics.h"

#include "combaseapi.h"
#include <iostream>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#define SAFE_RELEASE(p) { if ( (p) ) { (p)->Release(); (p) = 0; } }

#define GET_CONSTANT_BUFFER_OFFSET(p) ((sizeof(p) + 255) & (~255))

Graphics::Graphics(HWND a_HWnd)
 : m_HWindow(a_HWnd)
{

}

bool Graphics::Initialize()
{
	HRESULT hres;
	
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


	//Create the graphics device
	hres = D3D12CreateDevice(dxgiAdapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_Device));
	//Prob want to move this into a macro
	if (FAILED(hres))
	{
		printf("[GFX]: Failed to create GFX Device with min level 11!");
		return false;
	}

	//Create the Command Queue
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

	//Create the Back Buffer
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

	//Create the RTV Descriptor Heap
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeap = {};
	rtvDescriptorHeap.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescriptorHeap.NumDescriptors = FRAME_BUFFER_COUNT;
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

	//D3D12_DESCRIPTOR_RANGE cbDiscriptorRangeDesc[1];
	//cbDiscriptorRangeDesc[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	//cbDiscriptorRangeDesc[0].NumDescriptors = 1;
	//cbDiscriptorRangeDesc[0].BaseShaderRegister = 0;
	//cbDiscriptorRangeDesc[0].RegisterSpace = 0;
	//cbDiscriptorRangeDesc[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//D3D12_ROOT_DESCRIPTOR_TABLE cbDescriptorTable = {};
	//cbDescriptorTable.NumDescriptorRanges = 1;
	//cbDescriptorTable.pDescriptorRanges = cbDiscriptorRangeDesc;

	//D3D12_ROOT_PARAMETER rootParam[1];
	//rootParam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	//rootParam[0].DescriptorTable = cbDescriptorTable;
	//rootParam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

	D3D12_ROOT_DESCRIPTOR rootCBVDescriptor;
	rootCBVDescriptor.ShaderRegister = 0;
	rootCBVDescriptor.RegisterSpace = 0;

	D3D12_ROOT_PARAMETER rootParams[1];
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParams[0].Descriptor = rootCBVDescriptor;
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

	//Note(Stan): Most of this code down here is for a temporary triangle
	//Creating a Root Signature
	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
	rootSignatureDesc.NumParameters = 1;
	rootSignatureDesc.pParameters = rootParams;
	rootSignatureDesc.NumStaticSamplers = 0;
	rootSignatureDesc.pStaticSamplers = nullptr;
	rootSignatureDesc.Flags = 
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;

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

	//Compile the Pixel Shader
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

	//Create the Input Layout
	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA , 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA , 0}
	};

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc = {};
	inputLayoutDesc.NumElements = sizeof(inputLayout) / sizeof(D3D12_INPUT_ELEMENT_DESC);
	inputLayoutDesc.pInputElementDescs = inputLayout;

	//Note(Stan): Standard STENCIL_OP for now
	const D3D12_DEPTH_STENCILOP_DESC defaultStencilOp = { D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_COMPARISON_FUNC_ALWAYS };

	D3D12_DEPTH_STENCIL_DESC depthStencilDesc = {};
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	depthStencilDesc.StencilEnable = false;
	depthStencilDesc.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
	depthStencilDesc.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
	depthStencilDesc.FrontFace = defaultStencilOp;
	depthStencilDesc.BackFace = defaultStencilOp;

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
	psoDesc.DepthStencilState = depthStencilDesc;

	hres = m_Device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_DefaultPipelineState));
	if (FAILED(hres))
	{
		printf("[GFX]: Failed to create the Graphics Pipeline Object");
		return false;
	}

	TempVertex vertexList[] =
	{
		{{ -0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f }},
		{{  0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 1.0f, 1.0f }},
		{{ -0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 1.0f, 1.0f }},
		{{  0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f, 1.0f }},

		// right side face
		{{  0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f }},
		{{  0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 1.0f, 1.0f }},
		{{  0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f, 1.0f }},
		{{  0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f, 1.0f }},

		// left side face
		{{ -0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f, 1.0f }},
		{{ -0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 1.0f, 1.0f }},
		{{ -0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f, 1.0f }},
		{{ -0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f, 1.0f }},

		// back face
		{{  0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f, 1.0f }},
		{{ -0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 1.0f, 1.0f }},
		{{  0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f, 1.0f }},
		{{ -0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f, 1.0f }},

		// top face
		{{ -0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f }},
		{{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 1.0f, 1.0f }},
		{{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 1.0f, 1.0f }},
		{{ -0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f, 1.0f }},

		// bottom face
		{{  0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f, 1.0f }},
		{{ -0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 1.0f, 1.0f }},
		{{  0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 1.0f, 1.0f }},
		{{ -0.5f, -0.5f,  0.5f}, {0.0f, 1.0f, 0.0f, 1.0f }},
	};

	uint32_t vertexBufferSize = sizeof(vertexList);
	
	hres = m_Device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&m_VertexBuffer)
	);
	m_VertexBuffer->SetName(L"Vertex Buffer Default Heap");
	if (FAILED(hres))
	{
		printf("[GFX]: Failed create Vertex Buffer Default Heap!");
		return false;
	}

	ID3D12Resource* vertexBufferUploadHeap = {};
	hres = m_Device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertexBufferUploadHeap)
	);
	vertexBufferUploadHeap->SetName(L"Vertex Buffer Upload Heap");
	if (FAILED(hres))
	{
		printf("[GFX]: Failed create Vertex Buffer Upload Heap!");
		return false;
	}

	D3D12_SUBRESOURCE_DATA vertexData = {};
	vertexData.pData = reinterpret_cast<BYTE*>(vertexList);
	vertexData.RowPitch = vertexBufferSize;
	vertexData.SlicePitch = vertexBufferSize;

	UpdateSubresources(m_CommandList, m_VertexBuffer, vertexBufferUploadHeap, 0, 0, 1, &vertexData);

	m_CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_VertexBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));

	uint32_t indexList[] =
	{
		0, 1, 2,
		0, 3, 1,

		// left face
		4, 5, 6,
		4, 7, 5,

		// right face
		8, 9, 10,
		8, 11, 9,

		// back face
		12, 13, 14,
		12, 15, 13,

		// top face
		16, 17, 18,
		16, 19, 17,

		// bottom face
		20, 21, 22,
		20, 23, 21,
	};
	
	uint32_t indexBufferSize = sizeof(indexList);

	m_NumOfCubeIndices = indexBufferSize / (sizeof(DWORD));

	hres = m_Device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize),
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&m_IndexBuffer)
	);
	m_IndexBuffer->SetName(L"Index Buffer Default Heap");
	if (FAILED(hres))
	{
		printf("[GFX]: Failed create Index Buffer Default Heap!");
		return false;
	}

	ID3D12Resource* indexBufferUploadHeap;
	hres = m_Device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBufferUploadHeap)
	);
	indexBufferUploadHeap->SetName(L"Index Buffer Upload Heap");
	if (FAILED(hres))
	{
		printf("[GFX]: Failed create Index Buffer Upload Heap!");
		return false;
	}

	D3D12_SUBRESOURCE_DATA indexData = {};
	indexData.pData = reinterpret_cast<BYTE*>(indexList);
	indexData.RowPitch = indexBufferSize;
	indexData.SlicePitch = indexBufferSize;

	UpdateSubresources(m_CommandList, m_IndexBuffer, indexBufferUploadHeap, 0, 0, 1, &indexData);

	m_CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_IndexBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_INDEX_BUFFER));

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

	for (uint32_t i = 0; i < FRAME_BUFFER_COUNT; i++)
	{
		hres = m_Device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(1024 * 64),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_ConstantBufferUploadHeaps[i])
		);
		m_ConstantBufferUploadHeaps[i]->SetName(L"CB Upload Resource Heap");

		ZeroMemory(&m_CBPerObject, sizeof(m_CBPerObject));

		CD3DX12_RANGE readRange(0,0);

		hres = m_ConstantBufferUploadHeaps[i]->Map(0, &readRange, reinterpret_cast<void**>(&m_CBVGPUAdress[i]));

		memcpy(m_CBVGPUAdress[i], &m_CBPerObject, sizeof(m_CBPerObject));
		memcpy(m_CBVGPUAdress[i] + GET_CONSTANT_BUFFER_OFFSET(ConstantBufferPerObject), &m_CBPerObject, sizeof(m_CBPerObject));
	}

	//Note(Stan): Temp camera structure and cube positions for testing
	float fov = 45.0f * (3.14f / 180.f);
	float aspectRatio = WINDOW_WIDTH / WINDOW_HEIGHT;
	DirectX::XMMATRIX tempMatrix = DirectX::XMMatrixPerspectiveFovLH(fov, aspectRatio, 0.1f, 1000.0f);
	DirectX::XMStoreFloat4x4(&m_CameraProjMatrix, tempMatrix);

	m_CameraPos =		DirectX::XMFLOAT4(0.0f, 2.0f, -4.0f, 0.0f);
	m_CameraTarget =	DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	m_CameraUp =		DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);

	DirectX::XMVECTOR cPos = DirectX::XMLoadFloat4(&m_CameraPos);
	DirectX::XMVECTOR cTarg = DirectX::XMLoadFloat4(&m_CameraTarget);
	DirectX::XMVECTOR cUp = DirectX::XMLoadFloat4(&m_CameraUp);
	tempMatrix = DirectX::XMMatrixLookAtLH(cPos, cTarg, cUp);
	DirectX::XMStoreFloat4x4(&m_CameraViewMatrix, tempMatrix);

	m_Cube1Pos = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR posVec = DirectX::XMLoadFloat4(&m_Cube1Pos);

	tempMatrix = DirectX::XMMatrixTranslationFromVector(posVec);
	DirectX::XMStoreFloat4x4(&m_Cube1RotationMatrix, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&m_Cube1WorldMatrix, tempMatrix);

	m_Cube2PosOffset = DirectX::XMFLOAT4(1.5f, 0.0f, 0.0f, 0.0f);
	posVec = DirectX::XMVectorAdd(DirectX::XMLoadFloat4(&m_Cube1Pos), DirectX::XMLoadFloat4(&m_Cube2PosOffset));

	tempMatrix = DirectX::XMMatrixTranslationFromVector(posVec);
	DirectX::XMStoreFloat4x4(&m_Cube1RotationMatrix, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&m_Cube1WorldMatrix, tempMatrix);

	//for (uint32_t i = 0; i < FRAME_BUFFER_COUNT; i++)
	//{
	//	D3D12_DESCRIPTOR_HEAP_DESC cbHeapDescriptor = {};
	//	cbHeapDescriptor.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	//	cbHeapDescriptor.NumDescriptors = 1;
	//	cbHeapDescriptor.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	//	hres = m_Device->CreateDescriptorHeap(&cbHeapDescriptor, IID_PPV_ARGS(&m_CBDescriptorHeap[i]));
	//	if (FAILED(hres))
	//	{
	//		printf("[GFX]: Failed create Constant Buffer Descriptor Heap!");
	//		return false;
	//	}
	//}

	//for (uint32_t i = 0; i < FRAME_BUFFER_COUNT; i++)
	//{
	//	hres = m_Device->CreateCommittedResource(
	//		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
	//		D3D12_HEAP_FLAG_NONE,
	//		&CD3DX12_RESOURCE_DESC::Buffer(1024 * 64),
	//		D3D12_RESOURCE_STATE_GENERIC_READ,
	//		nullptr,
	//		IID_PPV_ARGS(&m_CBUploadHeap[i])
	//	);
	//	m_CBUploadHeap[i]->SetName(L"Constant Buffer Upload Heap");

	//	D3D12_CONSTANT_BUFFER_VIEW_DESC cbViewDesc = {};
	//	cbViewDesc.BufferLocation = m_CBUploadHeap[i]->GetGPUVirtualAddress();
	//	cbViewDesc.SizeInBytes = ((sizeof(CBColorMultiply) + 255) & (~255));
	//	m_Device->CreateConstantBufferView(&cbViewDesc, m_CBDescriptorHeap[i]->GetCPUDescriptorHandleForHeapStart());
	//
	//	ZeroMemory(&m_CBColorMultiplier, sizeof(m_CBColorMultiplier));

	//	CD3DX12_RANGE readRange(0, 0);
	//	hres = m_CBUploadHeap[i]->Map(0, &readRange, reinterpret_cast<void**>(&m_CBColorMultiplierGPUAdress[i]));
	//	memcpy(m_CBColorMultiplierGPUAdress[i], &m_CBColorMultiplier, sizeof(m_CBColorMultiplier));
	//}

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

	m_VertexBufferView.BufferLocation = m_VertexBuffer->GetGPUVirtualAddress();
	m_VertexBufferView.StrideInBytes = sizeof(TempVertex);
	m_VertexBufferView.SizeInBytes = vertexBufferSize;

	m_IndexBufferView.BufferLocation = m_IndexBuffer->GetGPUVirtualAddress();
	m_IndexBufferView.Format = DXGI_FORMAT_R32_UINT;
	m_IndexBufferView.SizeInBytes = indexBufferSize;

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

	return true;
}

void Graphics::Update()
{
	DirectX::XMMATRIX rotMatX = DirectX::XMMatrixRotationX(0.0001f);
	DirectX::XMMATRIX rotMatY = DirectX::XMMatrixRotationY(0.0002f);
	DirectX::XMMATRIX rotMatZ = DirectX::XMMatrixRotationZ(0.0003f);

	DirectX::XMMATRIX rotMat = DirectX::XMLoadFloat4x4(&m_Cube1RotationMatrix) * rotMatX * rotMatY * rotMatZ;
	DirectX::XMStoreFloat4x4(&m_Cube1RotationMatrix, rotMat);

	DirectX::XMMATRIX translationMatrix = DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat4(&m_Cube1Pos));
	DirectX::XMMATRIX worldMatrix = rotMat * translationMatrix;

	DirectX::XMStoreFloat4x4(&m_Cube1WorldMatrix, worldMatrix);

	DirectX::XMMATRIX viewMat = XMLoadFloat4x4(&m_CameraViewMatrix);
	DirectX::XMMATRIX projMat = XMLoadFloat4x4(&m_CameraProjMatrix);
	DirectX::XMMATRIX wvpMat = XMLoadFloat4x4(&m_Cube1WorldMatrix) * viewMat * projMat;
	DirectX::XMMATRIX transposed = DirectX::XMMatrixTranspose(wvpMat);
	DirectX::XMStoreFloat4x4(&m_CBPerObject.WVPMatrix, transposed);

	memcpy(m_CBVGPUAdress[m_FrameIndex], &m_CBPerObject, sizeof(m_CBPerObject));

	rotMatX = DirectX::XMMatrixRotationX(0.0003f);
	rotMatY = DirectX::XMMatrixRotationY(0.0002f);
	rotMatZ = DirectX::XMMatrixRotationZ(0.0001f);

	rotMat = rotMatZ * DirectX::XMLoadFloat4x4(&m_Cube2RotationMatrix) * (rotMatX * rotMatY);
	DirectX::XMStoreFloat4x4(&m_Cube2RotationMatrix, rotMat);

	DirectX::XMMATRIX translationOffsetMat = DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat4(&m_Cube2PosOffset));
	DirectX::XMMATRIX scaleMat = DirectX::XMMatrixScaling(0.5f, 0.5f, 0.5f);

	worldMatrix = scaleMat * translationOffsetMat /** rotMat */* translationMatrix;

	wvpMat = XMLoadFloat4x4(&m_Cube2WorldMatrix) * viewMat * projMat;
	transposed = DirectX::XMMatrixTranspose(wvpMat);
	DirectX::XMStoreFloat4x4(&m_CBPerObject.WVPMatrix, transposed);

	memcpy(m_CBVGPUAdress[m_FrameIndex] + GET_CONSTANT_BUFFER_OFFSET(ConstantBufferPerObject), &m_CBPerObject, sizeof(m_CBPerObject));

	DirectX::XMStoreFloat4x4(&m_Cube2WorldMatrix, worldMatrix);
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
	hres = m_CommandList->Reset(m_CommandAllocator[m_FrameIndex], m_DefaultPipelineState);
	if (FAILED(hres))
	{
		printf("[GFX]: Failed reset the Command list!");
		//TODO(Stan): Make a good way to cancel the update / graphics pipeline		
	}

	m_CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_RenderTargets[m_FrameIndex], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
	
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_RTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), m_FrameIndex, m_RTVDescriptorSize);
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsHandle(m_DSDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

	m_CommandList->OMSetRenderTargets(1, &rtvHandle, false, &dsHandle);

	FLOAT color[4]{ 0.32f, 0.40f, 0.45, 1.0f };
	m_CommandList->ClearRenderTargetView(rtvHandle, color, 0, nullptr);
	m_CommandList->ClearDepthStencilView(dsHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	m_CommandList->SetGraphicsRootSignature(m_RootSignature);

	//ID3D12DescriptorHeap* descriptorHeaps[] { m_CBDescriptorHeap[m_FrameIndex] };
	//m_CommandList->SetDescriptorHeaps(1, descriptorHeaps);
	//m_CommandList->SetGraphicsRootDescriptorTable(0, m_CBDescriptorHeap[m_FrameIndex]->GetGPUDescriptorHandleForHeapStart());

	m_CommandList->RSSetViewports(1, &m_Viewport);
	m_CommandList->RSSetScissorRects(1, &m_ScissorRect);
	m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_CommandList->IASetVertexBuffers(0, 1, &m_VertexBufferView);
	m_CommandList->IASetIndexBuffer(&m_IndexBufferView);

	m_CommandList->SetGraphicsRootConstantBufferView(0, m_ConstantBufferUploadHeaps[m_FrameIndex]->GetGPUVirtualAddress());
	m_CommandList->DrawIndexedInstanced(m_NumOfCubeIndices, 1, 0, 0, 0);

	m_CommandList->SetGraphicsRootConstantBufferView(0, m_ConstantBufferUploadHeaps[m_FrameIndex]->GetGPUVirtualAddress() + GET_CONSTANT_BUFFER_OFFSET(ConstantBufferPerObject));
	m_CommandList->DrawIndexedInstanced(m_NumOfCubeIndices, 1, 0, 0, 0);

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
		SAFE_RELEASE(m_ConstantBufferUploadHeaps[i])
	}

	SAFE_RELEASE(m_DefaultPipelineState);
	SAFE_RELEASE(m_RootSignature);
	SAFE_RELEASE(m_VertexBuffer);
	SAFE_RELEASE(m_IndexBuffer);

	SAFE_RELEASE(m_DepthStenil);
	SAFE_RELEASE(m_DSDescriptorHeap);
}