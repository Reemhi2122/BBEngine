#pragma once
#include "Graphics.h"

class DepthStencil
{
public:
	DepthStencil() = default;
	~DepthStencil();

	void Init(Graphics& a_Graphics);
	ID3D11ShaderResourceView*	GetResourceView() { return m_TextureDepthShaderResourceView; };
	ID3D11DepthStencilView*		GetDepthStencilView() { return m_TextureDepthStencilView; };

private:
	ID3D11Texture2D* m_TextureDepthTarget = nullptr;
	ID3D11DepthStencilView* m_TextureDepthStencilView = nullptr;
	ID3D11ShaderResourceView* m_TextureDepthShaderResourceView = nullptr;
};

inline void DepthStencil::Init(Graphics& a_Graphics)
{
	D3D11_TEXTURE2D_DESC tex_desc{};
	tex_desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	tex_desc.Width = 1600u;
	tex_desc.Height = 900u;
	tex_desc.MipLevels = 1u;
	tex_desc.ArraySize = 1u;
	tex_desc.SampleDesc.Count = 1u;
	tex_desc.SampleDesc.Quality = 0u;
	tex_desc.Usage = D3D11_USAGE_DEFAULT;
	tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	tex_desc.CPUAccessFlags = 0;
	tex_desc.MiscFlags = 0;

	HRESULT res = a_Graphics.GetDevice()->CreateTexture2D(&tex_desc, nullptr, &m_TextureDepthTarget);

	D3D11_DEPTH_STENCIL_VIEW_DESC TextureDepthStencilDesc{};
	TextureDepthStencilDesc.Flags = 0;
	TextureDepthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	TextureDepthStencilDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	TextureDepthStencilDesc.Texture2D.MipSlice = 0;

	res = a_Graphics.GetDevice()->CreateDepthStencilView(m_TextureDepthTarget, &TextureDepthStencilDesc, &m_TextureDepthStencilView);


	D3D11_SHADER_RESOURCE_VIEW_DESC TextureDepthShaderResourceViewDesc{};
	TextureDepthShaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	TextureDepthShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	TextureDepthShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	TextureDepthShaderResourceViewDesc.Texture2D.MipLevels = 1;

	res = a_Graphics.GetDevice()->CreateShaderResourceView(m_TextureDepthTarget, &TextureDepthShaderResourceViewDesc, &m_TextureDepthShaderResourceView);
}

inline DepthStencil::~DepthStencil() 
{
	delete m_TextureDepthTarget;
	m_TextureDepthTarget = nullptr;
	delete m_TextureDepthStencilView;
	m_TextureDepthStencilView = nullptr;
	delete m_TextureDepthShaderResourceView;
	m_TextureDepthShaderResourceView = nullptr;
}