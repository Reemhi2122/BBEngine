#pragma once
#include "Bindable.h"

#define CUBEMAP_SIZE 6

enum class CubeMapType
{
	TEXTURE,
	DEPTH
};

class CubeMap : public Bindable
{
public:

	CubeMap(Graphics& a_Gfx);
	CubeMap(Graphics& a_Gfx, CubeMapType a_Type, uint32_t a_Resolution = 1024u, char* a_TexturePaths = nullptr);

	ID3D11DepthStencilView** GetTextureDepthStencilViews() { return m_TextureDepthStencilViews; }
	ID3D11ShaderResourceView** GetTextureDepthRSV() { return m_TextureDepthSRV; }

	ID3D11ShaderResourceView* GetTextureDepthCMRSV(){ return m_ShaderResourceView; }

	void Bind(Graphics& a_Gfx) noexcept;

private:
	ID3D11Texture2D*			m_TextureCubeMap;
	ID3D11ShaderResourceView*	m_ShaderResourceView;

	ID3D11DepthStencilView* m_TextureDepthStencilViews[6];
	ID3D11ShaderResourceView* m_TextureDepthSRV[6];

	unsigned char* images[CUBEMAP_SIZE];
};