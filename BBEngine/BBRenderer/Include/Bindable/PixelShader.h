#pragma once
#include "Bindable/Bindable.h"

class PixelShader : public Bindable {
public:
	PixelShader(Graphics& a_Gfx, const std::wstring& a_Path);
	void Bind(Graphics& a_Gfx) noexcept;

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader;
};