#include "Bindable/CubeMap.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

CubeMap::CubeMap(Graphics& a_Gfx, const char* a_Path)
{
	int sizeX, sizeY, Channels;
	unsigned char* img = stbi_load(a_Path, &sizeX, &sizeY, &Channels, 4);

	HRESULT res;

	D3D11_TEXTURE2D_DESC textureCubeMapDesc = {};
	textureCubeMapDesc.Width = 640;
	textureCubeMapDesc.Height = 640;
	textureCubeMapDesc.MipLevels = 1;
	textureCubeMapDesc.ArraySize = 6;
	textureCubeMapDesc.SampleDesc.Count = 1u;
	textureCubeMapDesc.SampleDesc.Quality = 0u;
	textureCubeMapDesc.Usage = D3D11_USAGE_DEFAULT;
	textureCubeMapDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureCubeMapDesc.CPUAccessFlags = 0;
	textureCubeMapDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA image_subresource_data = {};
	image_subresource_data.pSysMem = img;
	image_subresource_data.SysMemPitch = sizeX * 4;

	res = a_Gfx.GetDevice()->CreateTexture2D(&textureCubeMapDesc, &image_subresource_data, &m_TextureCubeMap);

	D3D11_SHADER_RESOURCE_VIEW_DESC image_rsv_desc = {};
	image_rsv_desc.Format = textureCubeMapDesc.Format;
	image_rsv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	image_rsv_desc.Texture2D.MostDetailedMip = 0;
	image_rsv_desc.Texture2D.MipLevels = 1;

	res = a_Gfx.GetDevice()->CreateShaderResourceView(m_TextureCubeMap.Get(), &image_rsv_desc, &m_ShaderResourceView);
}

void CubeMap::Bind(Graphics& a_Gfx) noexcept
{
	a_Gfx.GetContext()->PSSetShaderResources(0, 1, m_ShaderResourceView.GetAddressOf());
}