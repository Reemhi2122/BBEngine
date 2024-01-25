#include "Bindable/Texture.h"

#include <combaseapi.h>

#include <wincodec.h>

Texture::Texture(Graphics& a_Gfx)
{
	//CoCreateInstance();

	CreateDecoderFromFilename();
}