#pragma once
#include "Bindable.h"

class CubeMap : public Bindable
{
public:
	CubeMap(Graphics& a_Gfx);
	void Bind(Graphics& a_Gfx) noexcept;

private:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_TextureCubeMap;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_ShaderResourceView;
};