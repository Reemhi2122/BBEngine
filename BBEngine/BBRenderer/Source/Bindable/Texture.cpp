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

}

void Texture::Bind(Graphics& a_Gfx) noexcept
{
	return;
}