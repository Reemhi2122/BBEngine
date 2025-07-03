#pragma once
#include "Bindable.h"

class ITexture : public Bindable
{
public:
	ITexture() = default;
	~ITexture() = default;

	virtual	bool Create(IGraphics& a_Gfx, const char* a_Path, uint32_t a_StartSlot = 0u, bool CreateRSV = true) = 0;
	virtual void Bind(IGraphics& a_Gfx) noexcept = 0;
	virtual void UnBind(IGraphics& a_Gfx) noexcept = 0;

private:
	uint32_t m_StartSlot;
};