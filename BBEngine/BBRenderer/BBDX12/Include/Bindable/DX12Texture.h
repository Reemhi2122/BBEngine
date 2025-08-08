#pragma once
#include "Bindable/ITexture.h"
#include "Graphics.h"

class SRVDescriptorInfo;

class DX12Texture : public ITexture
{
public:
	DX12Texture() = default;
	~DX12Texture() = default;

	bool Create(IGraphics& a_Gfx, const char* a_Path, uint32_t a_StartSlot = 0u, bool CreateRSV = true) override;
	void Bind(IGraphics& a_Gfx) noexcept override;
	void UnBind(IGraphics& a_Gfx) noexcept override;

	uintptr_t GetSRVGPUHandle() override;

	ID3D12Resource* GetTextureBuffer() const noexcept { return m_TextureBuffer; };

private:
	ID3D12Resource* m_TextureBuffer;
	ID3D12Resource* m_TextureUploadBufferHeap;

	SRVDescriptorInfo* m_DescriptorInfo;
};