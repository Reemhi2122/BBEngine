#pragma once
#include "Bindable.h"

class Texture : public Bindable
{
public:
	Texture() = default;
	Texture(Graphics& a_Gfx, const char* a_Path, uint32_t a_StartSlot = 0u);
	
	void Bind(Graphics& a_Gfx) noexcept;
	void UnBind(Graphics& a_Gfx) noexcept;

	ID3D11ShaderResourceView* GetRSV() { return m_ShaderResourceView.Get(); };

private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_ShaderResourceView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_Texture;

	uint32_t m_StartSlot;
};