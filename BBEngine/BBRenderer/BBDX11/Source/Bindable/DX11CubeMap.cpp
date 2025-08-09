#include "Bindable/DX11CubeMap.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

bool DX11CubeMap::Create(IGraphics& a_Gfx)
{
	constexpr char* names[CUBEMAP_SIZE] = {"right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "front.jpg", "back.jpg"};

	int sizeX, sizeY, Channels;
	for (uint32_t i = 0; i < CUBEMAP_SIZE; i++)
	{
		char path[MAX_PATH] = "Assets/Textures/skybox/";
		strcat(path, names[i]);
		images[i] = stbi_load(path, &sizeX, &sizeY, &Channels, 4);
		assert(images[i] != nullptr);
	}

	HRESULT res;
	D3D11_TEXTURE2D_DESC textureCubeMapDesc = {};
	textureCubeMapDesc.Width = sizeX;
	textureCubeMapDesc.Height = sizeY;
	textureCubeMapDesc.MipLevels = 1;
	textureCubeMapDesc.ArraySize = 6;
	textureCubeMapDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureCubeMapDesc.SampleDesc.Count = 1u;
	textureCubeMapDesc.SampleDesc.Quality = 0u;
	textureCubeMapDesc.Usage = D3D11_USAGE_DEFAULT;
	textureCubeMapDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureCubeMapDesc.CPUAccessFlags = 0;
	textureCubeMapDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	D3D11_SUBRESOURCE_DATA image_subresource_data[CUBEMAP_SIZE] = {};
	for (uint32_t i = 0; i < CUBEMAP_SIZE; i++)
	{
		image_subresource_data[i].pSysMem = images[i];
		image_subresource_data[i].SysMemPitch = sizeX * 4;
		image_subresource_data[i].SysMemSlicePitch = 0;
	}
	
	res = a_Gfx.GetDevice()->CreateTexture2D(&textureCubeMapDesc, image_subresource_data, &m_TextureCubeMap);

	D3D11_SHADER_RESOURCE_VIEW_DESC image_rsv_desc = {};
	image_rsv_desc.Format = textureCubeMapDesc.Format;
	image_rsv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	image_rsv_desc.Texture2D.MostDetailedMip = 0;
	image_rsv_desc.Texture2D.MipLevels = 1;

	res = a_Gfx.GetDevice()->CreateShaderResourceView(m_TextureCubeMap, &image_rsv_desc, &m_ShaderResourceView);

	return true;
}

bool DX11CubeMap::Create(IGraphics& a_Gfx, CubeMapType a_Type, uint32_t a_Resolution /*= 1024u*/, char* a_TexturePaths /*= nullptr*/)
{
	HRESULT res;
	D3D11_TEXTURE2D_DESC textureCubeMapDesc = {};
	textureCubeMapDesc.Width = a_Resolution;
	textureCubeMapDesc.Height = a_Resolution;
	textureCubeMapDesc.MipLevels = 1;
	textureCubeMapDesc.ArraySize = 6;
	textureCubeMapDesc.Format = (a_Type == CubeMapType::TEXTURE ? DXGI_FORMAT_R8G8B8A8_UNORM : DXGI_FORMAT_R24G8_TYPELESS);
	textureCubeMapDesc.SampleDesc.Count = 1u;
	textureCubeMapDesc.SampleDesc.Quality = 0u;
	textureCubeMapDesc.Usage = D3D11_USAGE_DEFAULT;
	textureCubeMapDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	textureCubeMapDesc.CPUAccessFlags = 0;
	textureCubeMapDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	res = a_Gfx.GetDevice()->CreateTexture2D(&textureCubeMapDesc, nullptr, &m_TextureCubeMap);
	
	for (uint32_t i = 0; i < CUBEMAP_SIZE; i++)
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC TextureDepthStencilDesc{};
		TextureDepthStencilDesc.Flags = 0;
		TextureDepthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		TextureDepthStencilDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
		TextureDepthStencilDesc.Texture2DArray.ArraySize = 1;
		TextureDepthStencilDesc.Texture2DArray.FirstArraySlice = i;
		TextureDepthStencilDesc.Texture2DArray.MipSlice = 0;
		
		res = a_Gfx.GetDevice()->CreateDepthStencilView(m_TextureCubeMap, &TextureDepthStencilDesc, &m_TextureDepthStencilViews[i]);
	}

	for (uint32_t i = 0; i < CUBEMAP_SIZE; i++)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC image_rsv_desc = {};
		image_rsv_desc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		image_rsv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		image_rsv_desc.Texture2DArray.ArraySize = 1;
		image_rsv_desc.Texture2DArray.FirstArraySlice = i;
		image_rsv_desc.Texture2DArray.MipLevels = 1;

		res = a_Gfx.GetDevice()->CreateShaderResourceView(m_TextureCubeMap, &image_rsv_desc, &m_TextureDepthSRV[i]);
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC image_rsv_desc = {};
	image_rsv_desc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	image_rsv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	image_rsv_desc.Texture2D.MostDetailedMip = 0;
	image_rsv_desc.Texture2D.MipLevels = 1;

	res = a_Gfx.GetDevice()->CreateShaderResourceView(m_TextureCubeMap, &image_rsv_desc, &m_ShaderResourceView);
	
	return true;
}

void DX11CubeMap::Bind(IGraphics& a_Gfx) noexcept
{
	a_Gfx.GetContext()->PSSetShaderResources(0, 1, &m_ShaderResourceView);
}