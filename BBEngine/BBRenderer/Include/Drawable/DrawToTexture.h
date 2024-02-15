#pragma once
#include "Bindable/BindableInclude.h"
#include "Drawable/DrawableBase.h"

class DrawToTexture : Drawable
{
public:
	DrawToTexture() = default;
	DrawToTexture(void* BitmapBuffer, VertexShader* a_VertexShader, PixelShader* a_PixelShader);
	~DrawToTexture() = default;

private:
	Graphics* m_Graphics;
};