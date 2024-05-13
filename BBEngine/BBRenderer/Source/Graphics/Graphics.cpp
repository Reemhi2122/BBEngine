#include "Graphics.h"
#include "Utils/GraphicsThrowMacros.h"

#include <sstream>
#include <d3dcompiler.h>

#include "Bindable/BindableInclude.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")


Graphics::Graphics(HWND a_HWnd)
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = a_HWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	HRESULT hr;

	GFX_THROW_FAILED(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&m_SwapChain,
		&m_Device,
		nullptr,
		&m_Context));

	Microsoft::WRL::ComPtr<ID3D11Resource> backBuffer;
	GFX_THROW_FAILED(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer));
	GFX_THROW_FAILED(m_Device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_Target));

	//Create depth stencil
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> dsState;
	GFX_THROW_FAILED(m_Device->CreateDepthStencilState(&dsDesc, &dsState));

	//Bind depth state
	m_Context->OMSetDepthStencilState(dsState.Get(), 1);

	// create depth stencil texture
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencil;
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = 1600u;
	descDepth.Height = 900u;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1u;
	descDepth.SampleDesc.Quality = 0u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	GFX_THROW_FAILED(m_Device->CreateTexture2D(&descDepth, nullptr, &depthStencil));

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0u;
	m_Device->CreateDepthStencilView(depthStencil.Get(), &descDSV, &m_DepthStencilView);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(a_HWnd);
	ImGui_ImplDX11_Init(m_Device.Get(), m_Context.Get());

	m_Context->OMSetRenderTargets(1u, m_Target.GetAddressOf(), m_DepthStencilView.Get());

	//Configure a viewport
	D3D11_VIEWPORT viewport;
	viewport.Width = 1600;
	viewport.Height = 900;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	m_Context->RSSetViewports(1, &viewport);

	m_Camera = new Camera();
}

Graphics::~Graphics() {
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void Graphics::EndFrame()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	HRESULT hr;
	if (FAILED(hr = m_SwapChain->Present(1u, 0u))) {
		if (hr == DXGI_ERROR_DEVICE_REMOVED) {
			GFX_DEVICE_REMOVED_EXCEPT(m_Device->GetDeviceRemovedReason());
		}
		else {
			GFX_THROW_FAILED(hr);
		}
	}
}

void Graphics::ClearBuffer(float a_Red, float a_Green, float a_Blue) noexcept
{
	const float color[] = { a_Red, a_Green, a_Blue, 1.0f };
	m_Context->ClearRenderTargetView(m_Target.Get(), color);
	m_Context->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void Graphics::DrawTestTriangle(float a_Angle, float x, float y, Vertex* ver, unsigned short* indices) {

	HRESULT hr;

	//const Vertex vertices[] = {
	//	{ -1.0f, -1.0f, -1.0f},
	//	{ 1.0f, -1.0f, -1.0f},
	//	{ -1.0f, 1.0f, -1.0f},
	//	{ 1.0f, 1.0f, -1.0f},
	//	{ -1.0f, -1.0f, 1.0f},
	//	{ 1.0f, -1.0f, 1.0f},
	//	{ -1.0f, 1.0f, 1.0f},
	//	{ 1.0f, 1.0f, 1.0f}
	//};

	Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer;
	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.ByteWidth = 11112;
	desc.StructureByteStride = sizeof(BBE::Vertex);
	D3D11_SUBRESOURCE_DATA source = {};
	source.pSysMem = ver;
	GFX_THROW_FAILED(m_Device->CreateBuffer(&desc, &source, &vertex_buffer));

	const UINT stride = sizeof(BBE::Vertex);
	const UINT offset = 0;
	m_Context->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);

	//Create index buffer
	//const unsigned short indicesOld[] = {
	//	0,2,1, 2,3,1,
	//	1,3,5, 3,7,5,
	//	2,6,3, 3,6,7,
	//	4,5,7, 4,7,6,
	//	0,4,2, 2,4,6,
	//	0,1,4, 1,5,4
	//};

	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.ByteWidth = 5232;
	ibd.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices;
	GFX_THROW_FAILED(m_Device->CreateBuffer(&ibd, &isd, &indexBuffer));

	m_Context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	//Create constant buffer for matrix
	struct ConstantBuffer {
		DirectX::XMMATRIX transform;
	};

	const ConstantBuffer cb = {
		{
			DirectX::XMMatrixTranspose(
				DirectX::XMMatrixRotationZ(a_Angle) *
				DirectX::XMMatrixRotationY(a_Angle) *
				DirectX::XMMatrixTranslation(x, y, 5.0f) *
				DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 10.0f)
			)
		}
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;
	D3D11_BUFFER_DESC cbd = {};
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0;
	cbd.ByteWidth = sizeof(cb);
	cbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &cb;
	GFX_THROW_FAILED(m_Device->CreateBuffer(&cbd, &csd, &constantBuffer));

	m_Context->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());

	struct ConstantBufferColor {
		struct {
			float r;
			float g;
			float b;
			float a;
		}face_colors[6];
	};

	const ConstantBufferColor cbc = {
		{
			{1.0f, 0.0f, 1.0f, 1.0f},
			{1.0f, 0.0f, 0.0f, 1.0f},
			{0.0f, 1.0f, 0.0f, 1.0f},
			{0.0f, 0.0f, 1.0f, 1.0f},
			{1.0f, 1.0f, 0.0f, 1.0f},
			{0.0f, 1.0f, 1.0f, 1.0f}
		}
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBufferColor;
	D3D11_BUFFER_DESC cbdc = {};
	cbdc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbdc.Usage = D3D11_USAGE_DEFAULT;
	cbdc.CPUAccessFlags = 0;
	cbdc.MiscFlags = 0;
	cbdc.ByteWidth = sizeof(cbc);
	cbdc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA csdc = {};
	csdc.pSysMem = &cbc;
	GFX_THROW_FAILED(m_Device->CreateBuffer(&cbdc, &csdc, &constantBufferColor));

	m_Context->PSSetConstantBuffers(0, 1, constantBufferColor.GetAddressOf());

	//Create blob
	Microsoft::WRL::ComPtr<ID3DBlob> blob;

	////Create pixel shader
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shader;
	D3DCompileFromFile(L"Assets/PixelShader.hlsl", nullptr, nullptr, "main", "ps_5_0", 0, 0, &blob, nullptr);
	m_Device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixel_shader);

	////Bind Pixel shader
	m_Context->PSSetShader(pixel_shader.Get(), nullptr, 0);

	//Create vertex shader
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex_shader;
	D3DCompileFromFile(L"Assets/VertexShader.hlsl", nullptr, nullptr, "main", "vs_5_0", 0, 0, &blob, nullptr);
	m_Device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertex_shader);

	//Bind the vertex shader
	m_Context->VSSetShader(vertex_shader.Get(), nullptr, 0);

	//Create input layout
	Microsoft::WRL::ComPtr<ID3D11InputLayout> input_layout;
	const D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"Position",0,DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	m_Device->CreateInputLayout(
		ied,
		(UINT)std::size(ied),
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		&input_layout); 

	//Bind vertex layout
	m_Context->IASetInputLayout(input_layout.Get());

	//Set primitive topology
	m_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Draw
	m_Context->DrawIndexed(36, 0, 0);
}

void Graphics::DrawIndexed(UINT a_Count) noexcept {
	m_Context->DrawIndexed(a_Count, 0, 0);
}

void Graphics::DrawIndexedInstanced(UINT a_IndexCount, UINT a_InstanceCount) noexcept {
	m_Context->DrawIndexedInstanced(a_IndexCount, a_InstanceCount, 0, 0, 0);
}

void Graphics::SetProjection(DirectX::XMMATRIX a_Projections) {
	m_Projection = a_Projections;
}

DirectX::XMMATRIX Graphics::GetProjection() const noexcept {
	return m_Projection;
}

void Graphics::ResetRenderTarget() {
	m_Context->OMSetRenderTargets(1u, m_Target.GetAddressOf(), m_DepthStencilView.Get());
}