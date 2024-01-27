#include "Bindable/Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(Graphics& a_Gfx)
{
	int sizeX, sizeY, Channels;
	unsigned char* img = stbi_load("D:\\PersonalProjects\\BBEngine\\BBEngine\\BBGame\\Assets\\Models\\Lantern\\glTF\\Lantern_baseColor.png", &sizeX, &sizeY, &Channels, 0);

	D3D11_TEXTURE2D_DESC desc;
	desc.Width = sizeX;
	desc.Height = sizeY;
	desc.MipLevels = desc.ArraySize = 1;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;

	a_Gfx.GetDevice()->CreateTexture2D(&desc, NULL, m_Texture.GetAddressOf());

	a_Gfx.GetDevice()->CreateShaderResourceView(m_Texture.Get(), nullptr, m_ShaderResourceView.GetAddressOf());

	D3D11_SAMPLER_DESC image_sampler_desc;
	image_sampler_desc.Filter = D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
	image_sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	image_sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	image_sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	image_sampler_desc.MipLODBias = 0.0f;
	image_sampler_desc.MaxAnisotropy = 1;
	image_sampler_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	image_sampler_desc.BorderColor[0] = 1.0f;
	image_sampler_desc.BorderColor[1] = 1.0f;
	image_sampler_desc.BorderColor[2] = 1.0f;
	image_sampler_desc.BorderColor[3] = 1.0f;
	image_sampler_desc.MinLOD = -FLT_MAX;
	image_sampler_desc.MaxLOD = FLT_MAX;

	a_Gfx.GetDevice()->CreateSamplerState(&image_sampler_desc, m_SamplerState.GetAddressOf());

}

void Texture::Bind(Graphics& a_Gfx) noexcept
{
	a_Gfx.GetContext()->PSSetShaderResources(0, 1, m_ShaderResourceView.GetAddressOf());
	a_Gfx.GetContext()->PSSetSamplers(0, 1, m_SamplerState.GetAddressOf());
}