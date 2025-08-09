#pragma once
#include "Bindable/ICubeMap.h"

#define CUBEMAP_SIZE 6

enum class CubeMapType
{
	TEXTURE,
	DEPTH
};

class DX11CubeMap : public ICubeMap
{
public:
	DX11CubeMap() = default;
	~DX11CubeMap() = default;

	bool Create(IGraphics& a_Gfx) override;
	bool Create(IGraphics& a_Gfx, CubeMapType a_Type, uint32_t a_Resolution = 1024u, char* a_TexturePaths = nullptr) override;

	ID3D11DepthStencilView**	GetTextureDepthStencilViews() { return m_TextureDepthStencilViews; }
	ID3D11ShaderResourceView**	GetTextureDepthRSV() { return m_TextureDepthSRV; }

	ID3D11ShaderResourceView*	GetTextureDepthCMRSV(){ return m_ShaderResourceView; }
	ID3D11Texture2D*			GetRawTextureArray() { return m_TextureCubeMap; };

	void Bind(IGraphics& a_Gfx) noexcept;

private:
	ID3D11Texture2D*			m_TextureCubeMap;
	ID3D11ShaderResourceView*	m_ShaderResourceView;

	ID3D11DepthStencilView* m_TextureDepthStencilViews[6];
	ID3D11ShaderResourceView* m_TextureDepthSRV[6];

	unsigned char* images[CUBEMAP_SIZE];
};