#pragma once
#include "Bindable.h"

#define CUBEMAP_SIZE 6

class CubeMap : public Bindable
{
public:
	CubeMap(Graphics& a_Gfx, bool a_CreateStandardCubeMap);
	void Bind(Graphics& a_Gfx) noexcept;

private:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_TextureCubeMap;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_ShaderResourceView;

	unsigned char* images[CUBEMAP_SIZE];
};