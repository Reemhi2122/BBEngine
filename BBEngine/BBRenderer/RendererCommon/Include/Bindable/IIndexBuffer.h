#pragma once
#include "Bindable/Bindable.h"

class IIndexBuffer : public Bindable
{
public:
	IIndexBuffer() = default;
	~IIndexBuffer() = default;

	virtual void Create(IGraphics& a_Gfx, uint8_t* a_Indices, const uint32_t a_Count, uint8_t a_IndexDataSize) = 0;
	virtual void Bind(IGraphics& a_Gfx) noexcept override = 0;
	virtual UINT GetCount() const noexcept = 0;

protected:
	UINT m_Count;
};