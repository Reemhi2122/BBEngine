#pragma once
#include "Bindable/Bindable.h"

class DX11Texture : public Bindable
{
public:
	DX11Texture() = default;
	DX11Texture(IGraphics& a_Gfx, const char* a_Path, uint32_t a_StartSlot = 0u);
	
	void Bind(IGraphics& a_Gfx) noexcept;
	void UnBind(IGraphics& a_Gfx) noexcept;

	ID3D11ShaderResourceView* GetRSV() { return m_ShaderResourceView; };

private:
	ID3D11ShaderResourceView* m_ShaderResourceView;
	ID3D11Texture2D* m_Texture;

	uint32_t m_StartSlot;
};