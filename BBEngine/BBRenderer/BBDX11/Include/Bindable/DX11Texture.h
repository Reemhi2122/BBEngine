#pragma once
#include "Bindable/ITexture.h"

class DX11Texture : public ITexture
{
public:
	DX11Texture() = default;

	bool Create(IGraphics& a_Gfx, const char* a_Path, uint32_t a_StartSlot = 0u, bool CreateRSV = true) override;
	void Bind(IGraphics& a_Gfx) noexcept override;
	void UnBind(IGraphics& a_Gfx) noexcept override;

	ID3D11ShaderResourceView* GetSRVGPUHandle() { return m_ShaderResourceView; };

private:
	ID3D11ShaderResourceView* m_ShaderResourceView;
	ID3D11Texture2D* m_Texture;

	uint32_t m_StartSlot;
};