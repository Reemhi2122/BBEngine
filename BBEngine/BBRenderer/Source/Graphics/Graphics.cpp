#include "Graphics.h"
#include "Utils/GraphicsThrowMacros.h"

#include <sstream>
#include <d3dcompiler.h>

#include "Bindable/BindableInclude.h"
#include "Drawable/Model.h"

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

#if 0
	D3D11_TEXTURE2D_DESC tex_desc;
	ZeroMemory(&tex_desc, sizeof(tex_desc));

	tex_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	tex_desc.Width = 1600u;
	tex_desc.Height = 900u;
	tex_desc.MipLevels = 1u;
	tex_desc.ArraySize = 1u;
	tex_desc.SampleDesc.Count = 1u;
	tex_desc.Usage = D3D11_USAGE_DEFAULT;
	tex_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	tex_desc.CPUAccessFlags = 0u;
	tex_desc.MiscFlags = 0u;

	HRESULT res = m_Device->CreateTexture2D(&tex_desc, nullptr, &m_TextureTarget);

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	renderTargetViewDesc.Format = tex_desc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	m_Device->CreateRenderTargetView(m_TextureTarget, &renderTargetViewDesc, &m_TextureRenderTargetView);

	shaderResourceViewDesc.Format = tex_desc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	m_Device->CreateShaderResourceView(m_TextureTarget, &shaderResourceViewDesc, &m_TextureShaderResourceView);
#endif

	D3D11_SAMPLER_DESC image_sampler_desc = {};
	image_sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	image_sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	image_sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	image_sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	image_sampler_desc.MaxAnisotropy = 1;
	image_sampler_desc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	image_sampler_desc.BorderColor[0] = 1.0f;
	image_sampler_desc.BorderColor[1] = 1.0f;
	image_sampler_desc.BorderColor[2] = 1.0f;
	image_sampler_desc.BorderColor[3] = 1.0f;
	image_sampler_desc.MinLOD = -FLT_MAX;
	image_sampler_desc.MaxLOD = FLT_MAX;

	m_Device->CreateSamplerState(&image_sampler_desc, &m_DepthTextureSampler);

	//Create depth stencil
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
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

TMPHANDLE Graphics::CreateShader(ShaderType a_Type, std::string a_Path, std::string a_EntryPointFunc /*= "main"*/)
{
	TMPHANDLE handle = -1;
	wchar_t pathString[MAX_PATH];
	mbstowcs(pathString, a_Path.c_str(), MAX_PATH);

	switch (a_Type)
	{
	case ShaderType::VertexShader:
	{
		D3DCompileFromFile(pathString, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, a_EntryPointFunc.c_str(), "vs_5_0", 0, 0, &m_VertexShaders[m_VertexIndex].m_ByteCodeBlob, nullptr);
		m_Device->CreateVertexShader(m_VertexShaders[m_VertexIndex].m_ByteCodeBlob->GetBufferPointer(), m_VertexShaders[m_VertexIndex].m_ByteCodeBlob->GetBufferSize(), nullptr, &m_VertexShaders[m_VertexIndex].m_Shader);
		m_VertexShaders[m_VertexIndex].m_Path = a_Path;
		handle = m_VertexIndex++;
	} break;
	case ShaderType::PixelShader:
	{
		Microsoft::WRL::ComPtr<ID3DBlob> blob;
		D3DCompileFromFile(pathString, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, a_EntryPointFunc.c_str(), "ps_5_0", D3DCOMPILE_DEBUG, 0, &blob, nullptr);
		m_Device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &m_PixelShaders[m_PixelIndex].m_Shader);
		m_PixelShaders[m_PixelIndex].m_Path = a_Path;
		handle = m_PixelIndex++;
	} break;
	case ShaderType::GeometryShader:
		break;
	case ShaderType::ComputeShader:
		break;
	case ShaderType::HullShader:
		break;
	default:
		break;
	}

	return handle;
}

void Graphics::BindShader(ShaderType a_Type, TMPHANDLE a_Shader)
{
	switch (a_Type)
	{
	case ShaderType::VertexShader:
		m_Context->VSSetShader(m_VertexShaders[a_Shader].m_Shader.Get(), nullptr, 0);
		break;
	case ShaderType::PixelShader:
		m_Context->PSSetShader(m_PixelShaders[a_Shader].m_Shader.Get(), nullptr, 0);
		break;
	case ShaderType::GeometryShader:
		break;
	case ShaderType::ComputeShader:
		break;
	case ShaderType::HullShader:
		break;
	default:
		break;
	}
}

void Graphics::ReloadShader(ShaderType a_Type, TMPHANDLE a_Shader)
{
	switch (a_Type)
	{
	case ShaderType::VertexShader:
	{
		VertexShader& vShader = m_VertexShaders[a_Shader];
		wchar_t pathString[MAX_PATH];
		mbstowcs(pathString, vShader.m_Path.c_str(), MAX_PATH);
		D3DCompileFromFile(pathString, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", 0, 0, &vShader.m_ByteCodeBlob, nullptr);
		m_Device->CreateVertexShader(vShader.m_ByteCodeBlob->GetBufferPointer(), vShader.m_ByteCodeBlob->GetBufferSize(), nullptr, &vShader.m_Shader);
	} break;
	case ShaderType::PixelShader:
	{
		PixelShader& pShader = m_PixelShaders[a_Shader];
		Microsoft::WRL::ComPtr<ID3DBlob> blob;
		wchar_t pathString[MAX_PATH];
		mbstowcs(pathString, pShader.m_Path.c_str(), MAX_PATH);
		D3DCompileFromFile(pathString, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", D3DCOMPILE_DEBUG, 0, &blob, nullptr);
		m_Device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pShader.m_Shader);
;
	} break;
	case ShaderType::GeometryShader:
		break;
	case ShaderType::ComputeShader:
		break;
	case ShaderType::HullShader:
		break;
	default:
		break;
	}
}

Microsoft::WRL::ComPtr<ID3DBlob> Graphics::GetVertexShaderByteCode(TMPHANDLE a_Shader) const
{
	return m_VertexShaders[a_Shader].m_ByteCodeBlob;
}

Graphics::~Graphics() 
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void Graphics::SetGameViewRenderTarget() 
{
	m_Context->OMSetRenderTargets(1u, &m_TextureRenderTargetView, m_DepthStencilView.Get());
	const float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_Context->ClearRenderTargetView(m_TextureRenderTargetView, color);
	m_Context->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void Graphics::SetDepthStencilTarget(ID3D11DepthStencilView* a_Target)
{
	m_Context->OMSetRenderTargets(0u, 0, a_Target);
	m_Context->ClearDepthStencilView(a_Target, D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void Graphics::BindDepthSampler() 
{
	m_Context->PSSetSamplers(1, 1, &m_DepthTextureSampler);
}

void Graphics::BindDepthTexture(ID3D11ShaderResourceView* a_View, uint32_t a_StartSlot, uint32_t a_NumViews)
{
	m_Context->PSSetShaderResources(a_StartSlot, a_NumViews, &a_View);
}

void Graphics::BindSampler(ID3D11SamplerState* a_Sampler, uint32_t a_StartSlot, uint32_t a_NumViews)
{
	m_Context->PSSetSamplers(a_StartSlot, a_NumViews, &a_Sampler);
}

void Graphics::UnbindSRV(uint32_t a_Slot) 
{
	ID3D11ShaderResourceView* nullSRV[1u] = { nullptr };
	m_Context->PSSetShaderResources(a_Slot, 1u, nullSRV);
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

void Graphics::DrawIndexed(UINT a_Count) noexcept 
{
	m_Context->DrawIndexed(a_Count, 0, 0);
}

void Graphics::DrawIndexedInstanced(UINT a_IndexCount, UINT a_InstanceCount) noexcept 
{
	m_Context->DrawIndexedInstanced(a_IndexCount, a_InstanceCount, 0, 0, 0);
}

void Graphics::SetProjection(DirectX::XMMATRIX a_Projections) 
{
	m_Projection = a_Projections;
}

DirectX::XMMATRIX Graphics::GetProjection() const noexcept 
{
	return m_Projection;
}

void Graphics::ResetRenderTarget() 
{
	m_Context->OMSetRenderTargets(1u, m_Target.GetAddressOf(), m_DepthStencilView.Get());
}

void Graphics::CreatePointLightDepthCubeMapArray()
{
	INFOMAN;
	D3D11_TEXTURE2D_DESC textureCubeMapDesc = {};
	textureCubeMapDesc.Width = 1024;
	textureCubeMapDesc.Height = 1024;
	textureCubeMapDesc.MipLevels = 1;
	textureCubeMapDesc.ArraySize = 120;
	textureCubeMapDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	textureCubeMapDesc.SampleDesc.Count = 1u;
	textureCubeMapDesc.SampleDesc.Quality = 0u;
	textureCubeMapDesc.Usage = D3D11_USAGE_DEFAULT;
	textureCubeMapDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	textureCubeMapDesc.CPUAccessFlags = 0;
	textureCubeMapDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	GFX_THROW_FAILED(m_Device->CreateTexture2D(&textureCubeMapDesc, nullptr, &m_PointLightDepthCubeArray));

	for (uint32_t i = 0; i < CUBEMAP_SIZE; i++)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC image_rsv_desc = {};
		image_rsv_desc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		image_rsv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		image_rsv_desc.Texture2DArray.ArraySize = 1;
		image_rsv_desc.Texture2DArray.FirstArraySlice = 6 + i;
		image_rsv_desc.Texture2DArray.MipLevels = 1;

		GFX_THROW_FAILED(m_Device->CreateShaderResourceView(m_PointLightDepthCubeArray, &image_rsv_desc, &m_TextureDepthSRV[i]));
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC RSV_desc = {};
	RSV_desc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	RSV_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
	RSV_desc.TextureCubeArray.MostDetailedMip = 0;
	RSV_desc.TextureCubeArray.MipLevels = 1;
	RSV_desc.TextureCubeArray.First2DArrayFace = 0;
	RSV_desc.TextureCubeArray.NumCubes = 12;

	GFX_THROW_FAILED(m_Device->CreateShaderResourceView(m_PointLightDepthCubeArray, &RSV_desc, &m_PointLightDepthCubeArraySRV));
}

void Graphics::CreatePointLightDepthCubeMap(ID3D11DepthStencilView** a_DepthStencilArray, uint32_t index)
{
	INFOMAN;
	for (uint32_t i = 0; i < CUBEMAP_SIZE; i++)
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC TextureDepthStencilDesc{};
		TextureDepthStencilDesc.Flags = 0;
		TextureDepthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		TextureDepthStencilDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
		TextureDepthStencilDesc.Texture2DArray.ArraySize = 1;
		TextureDepthStencilDesc.Texture2DArray.FirstArraySlice = (CUBEMAP_SIZE * index) + i;
		TextureDepthStencilDesc.Texture2DArray.MipSlice = 0;

		GFX_THROW_FAILED(m_Device->CreateDepthStencilView(m_PointLightDepthCubeArray, &TextureDepthStencilDesc, &a_DepthStencilArray[i]));
	}
}

void Graphics::CreateSpotLightDepthMapArray()
{
	INFOMAN;

	D3D11_TEXTURE2D_DESC tex_desc{};
	tex_desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	tex_desc.Width = 1024;
	tex_desc.Height = 1024;
	tex_desc.MipLevels = 1u;
	tex_desc.ArraySize = 120u;
	tex_desc.SampleDesc.Count = 1u;
	tex_desc.SampleDesc.Quality = 0u;
	tex_desc.Usage = D3D11_USAGE_DEFAULT;
	tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	tex_desc.CPUAccessFlags = 0;
	tex_desc.MiscFlags = 0;

	GFX_THROW_FAILED(m_Device->CreateTexture2D(&tex_desc, nullptr, &m_SpotLightsDepthArray));

	D3D11_SHADER_RESOURCE_VIEW_DESC image_rsv_desc = {};
	image_rsv_desc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	image_rsv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	image_rsv_desc.Texture2DArray.ArraySize = 1;
	image_rsv_desc.Texture2DArray.FirstArraySlice = 0;
	image_rsv_desc.Texture2DArray.MipLevels = 1;

	GFX_THROW_FAILED(m_Device->CreateShaderResourceView(m_SpotLightsDepthArray, &image_rsv_desc, &m_SpotLightsDepthTest));

	D3D11_SHADER_RESOURCE_VIEW_DESC TextureDepthShaderResourceViewDesc{};
	TextureDepthShaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	TextureDepthShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	TextureDepthShaderResourceViewDesc.Texture2DArray.MostDetailedMip = 0;
	TextureDepthShaderResourceViewDesc.Texture2DArray.MipLevels = 1;
	TextureDepthShaderResourceViewDesc.Texture2DArray.FirstArraySlice = 0;
	TextureDepthShaderResourceViewDesc.Texture2DArray.ArraySize = 120;

	GFX_THROW_FAILED(m_Device->CreateShaderResourceView(m_SpotLightsDepthArray, &TextureDepthShaderResourceViewDesc, &m_SpotLightsDepthArraySRV));
}

void Graphics::CreateSpotLightDepthTexture(ID3D11DepthStencilView** a_DepthStencilArray, uint32_t index)
{
	INFOMAN;

	D3D11_DEPTH_STENCIL_VIEW_DESC TextureDepthStencilDesc{};
	TextureDepthStencilDesc.Flags = 0;
	TextureDepthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	TextureDepthStencilDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
	TextureDepthStencilDesc.Texture2DArray.ArraySize = 1;
	TextureDepthStencilDesc.Texture2DArray.FirstArraySlice = index;
	TextureDepthStencilDesc.Texture2DArray.MipSlice = 0;

	GFX_THROW_FAILED(m_Device->CreateDepthStencilView(m_SpotLightsDepthArray, &TextureDepthStencilDesc, a_DepthStencilArray));
}

//////////////////////////////
//	Old Code
//////////////////////////////

void Graphics::DrawTestTriangle(float a_Angle, float x, float y, Vertex* ver, unsigned short* indices) 
{
	INFOMAN;

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