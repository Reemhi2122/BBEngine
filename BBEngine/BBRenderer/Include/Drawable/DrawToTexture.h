#pragma once
#include "Bindable/BindableInclude.h"
#include "Drawable/DrawableBase.h"

class DrawToTexture : Drawable
{
public:
	DrawToTexture() = default;
	DrawToTexture(Graphics& a_Graphics, VertexShader* a_VertexShader, PixelShader* a_PixelShader);
	~DrawToTexture() = default;

	void Update(float a_DeltaTime) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;

	const std::vector<Bindable*>& GetStaticBinds() const noexcept override
	{
		return std::vector<Bindable*>();
	}

private:
	Graphics* m_Graphics;
};