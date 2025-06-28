#include "Bindable.h"

class ITexture : public Bindable
{
	ITexture() = default;
	~ITexture() = default;

	virtual void Create(IGraphics& a_Gfx, const char* a_Path, uint32_t a_StartSlot = 0u) = 0;
	virtual void Bind(IGraphics& a_Gfx) noexcept = 0;
	virtual void UnBind(IGraphics& a_Gfx) noexcept = 0;

private:
	uint32_t m_StartSlot;
};