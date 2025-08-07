#pragma once
#include "Bindable/IIndexBuffer.h"
#include <vector>

class DX11IndexBuffer : public IIndexBuffer
{
public:
	DX11IndexBuffer() = default;

	bool Create(IGraphics& a_Gfx, uint8_t* a_Indices, const uint32_t a_Count, uint8_t a_IndexDataSize) override;
	void Bind(IGraphics& a_Gfx) noexcept override;
	UINT GetCount() const noexcept;

private:
	UINT m_Count;
	DXGI_FORMAT m_IndexBufferFormat;
	ID3D11Buffer* m_CPIndexBuffer;
};