#include "Bindable/Texture.h"
#include "stb_image.h"

Texture::Texture(IGraphics& a_Gfx, const char* a_Path, uint32_t a_StartSlot)
	: m_StartSlot(a_StartSlot)
{
	
}

void Texture::Bind(IGraphics& a_Gfx) noexcept
{

}

void Texture::UnBind(IGraphics& a_Gfx) noexcept
{

}