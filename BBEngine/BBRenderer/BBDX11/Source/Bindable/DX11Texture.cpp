#include "Bindable/DX11Texture.h"
#include "stb_image.h"

DX11Texture::DX11Texture(IGraphics& a_Gfx, const char* a_Path, uint32_t a_StartSlot)
	: m_StartSlot(a_StartSlot)
{
	int sizeX, sizeY, Channels;
	unsigned char* img = stbi_load(a_Path, &sizeX, &sizeY, &Channels, 4);

	HRESULT res;
	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = sizeX;
	desc.Height = sizeY;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA image_subresource_data = {};
	image_subresource_data.pSysMem = img;
	image_subresource_data.SysMemPitch = sizeX * 4;

	res = a_Gfx.GetDevice()->CreateTexture2D(&desc, &image_subresource_data, &m_Texture);

	D3D11_SHADER_RESOURCE_VIEW_DESC image_rsv_desc = {};
	image_rsv_desc.Format = desc.Format;
	image_rsv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	image_rsv_desc.Texture2D.MostDetailedMip = 0;
	image_rsv_desc.Texture2D.MipLevels = 1;

	res = a_Gfx.GetDevice()->CreateShaderResourceView(m_Texture, &image_rsv_desc, &m_ShaderResourceView);
}

void DX11Texture::Bind(IGraphics& a_Gfx) noexcept
{
	BBShaderResourceView* resourceViews[]{ m_ShaderResourceView };
	a_Gfx.GetContext()->PSSetShaderResources(m_StartSlot, 1, resourceViews);
}

void DX11Texture::UnBind(IGraphics& a_Gfx) noexcept
{
	ID3D11ShaderResourceView* nullSRV[1]{ nullptr };
	a_Gfx.GetContext()->PSSetShaderResources(m_StartSlot, 1, nullSRV);
}