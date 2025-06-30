#pragma once
#include "Bindable.h"

template<typename T>
class IConstantBuffer : public Bindable
{
public:
	IConstantBuffer() = default;
	~IConstantBuffer() = default;

	virtual bool Create(IGraphics& a_Gfx, uint32_t a_StartSlot = 0u, uint32_t a_NumBuffer = 1u) = 0;
	virtual void Update(IGraphics& a_Gfx, T& a_ConstantBuffer) = 0;
	virtual void Update(IGraphics& a_Gfx) = 0;
	virtual void Bind(IGraphics& a_Gfx) noexcept = 0;
};