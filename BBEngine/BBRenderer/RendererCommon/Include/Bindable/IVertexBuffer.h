#pragma once
#include "Bindable.h"

class IVertexBuffer : public Bindable
{
public:
	IVertexBuffer() = default;
	~IVertexBuffer() = default;

	virtual void Create(IGraphics& a_Gfx, void* a_Vertices, const uint32_t vbSize, const uint32_t a_Count) = 0;
	virtual void Bind(IGraphics& a_Gfx) noexcept = 0;
	virtual UINT GetCount() const noexcept = 0;

protected:
	UINT m_Count;
};