#include "Bindable/ITexture.h"

class DX12Texture : public ITexture
{
	DX12Texture() = default;
	~DX12Texture() = default;

	void Create(IGraphics& a_Gfx, const char* a_Path, uint32_t a_StartSlot = 0u) override;
	void Bind(IGraphics& a_Gfx) noexcept override;
	void UnBind(IGraphics& a_Gfx) noexcept override;
};