#pragma once
#include "Bindable/ICubeMap.h"
#include "Graphics.h"

class DX12CubeMap : public ICubeMap
{
public:
	DX12CubeMap() = default;
	~DX12CubeMap() = default;

	bool Create(IGraphics& a_Gfx) override;
	bool Create(IGraphics& a_Gfx, CubeMapType a_Type, uint32_t a_Resolution = 1024u, char* a_TexturePaths = nullptr) override;

	void Bind(IGraphics& a_Gfx) noexcept;

private:
	ID3D12Resource* m_TextureBuffer;
	ID3D12Resource* m_TextureUploadBufferHeap;

	SRVDescriptorInfo* m_DescriptorInfo;

	unsigned char* images[CUBEMAP_SIZE];
};