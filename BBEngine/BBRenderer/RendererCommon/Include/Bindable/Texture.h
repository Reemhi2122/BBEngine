#pragma once
#include "Bindable.h"
#include "RenderDefinitions.h"

class Texture : public Bindable
{
public:
	Texture() = default;
	Texture(IGraphics& a_Gfx, const char* a_Path, uint32_t a_StartSlot = 0u);

	void Bind(IGraphics& a_Gfx) noexcept;
	void UnBind(IGraphics& a_Gfx) noexcept;

	BBShaderResourceView* GetRSV() { return m_ShaderResourceView.Get(); };

private:
	Microsoft::WRL::ComPtr<BBShaderResourceView> m_ShaderResourceView;
	Microsoft::WRL::ComPtr<BBTexture2D> m_Texture;

	uint32_t m_StartSlot;
};