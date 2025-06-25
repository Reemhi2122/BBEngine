#pragma once
#include "Bindable/IIndexBuffer.h"

class DX12IndexBuffer : public IIndexBuffer
{
public:
	DX12IndexBuffer() = default;
	~DX12IndexBuffer() = default;

	void Create(IGraphics& a_Gfx, uint8_t* a_Indices, const uint32_t a_Count, uint8_t a_IndexDataSize);
	void Bind(IGraphics& a_Gfx) noexcept override;
	UINT GetCount() const noexcept;

protected:
	ID3D12Resource*				m_IndexBuffer;
	D3D12_INDEX_BUFFER_VIEW 	m_IndexBufferView;

	UINT m_Count;
};