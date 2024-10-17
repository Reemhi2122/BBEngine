#include "Bindable/CubeMap.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

CubeMap::CubeMap(Graphics& a_Gfx, bool a_CreateStandardCubeMap)
{
	constexpr char* names[CUBEMAP_SIZE] = {"right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "front.jpg", "back.jpg"};

	int sizeX, sizeY, Channels;
	for (uint32_t i = 0; i < CUBEMAP_SIZE; i++)
	{
		char path[MAX_PATH] = "Assets/Textures/skybox/";
		std::strcat(path, names[i]);
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

	res = a_Gfx.GetDevice()->CreateShaderResourceView(m_TextureCubeMap.Get(), &image_rsv_desc, &m_ShaderResourceView);
}

void CubeMap::Bind(Graphics& a_Gfx) noexcept
{
	a_Gfx.GetContext()->PSSetShaderResources(0, 1, m_ShaderResourceView.GetAddressOf());
}