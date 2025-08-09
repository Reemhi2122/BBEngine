#pragma once

#if 0

#include "Bindable/BindableInclude.h"
#include "Drawable/DrawableBase.h"

class DrawToTexture : Drawable
{
public:
	DrawToTexture() = default;
	DrawToTexture(Graphics& a_Graphics, uint32_t a_VertexShader, uint32_t a_PixelShader);
	~DrawToTexture() = default;

	void Update(float a_DeltaTime) noexcept override;

	const std::vector<Bindable*>& GetStaticBinds() const noexcept override
	{
		return std::vector<Bindable*>();
	}

private:
	Graphics* m_Graphics;
};

#endif
