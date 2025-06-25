#pragma once
#include "Bindable/IVertexBuffer.h"

class DX12VertexBuffer : public IVertexBuffer
{
public:
	DX12VertexBuffer() = default;
	~DX12VertexBuffer() = default;

	void Create(IGraphics& a_Gfx, void* a_Vertices, const uint32_t vbSize, const uint32_t a_Count) override;
	void Bind(IGraphics& a_Gfx) noexcept override;
	UINT GetCount() const noexcept override;

private:
	ID3D12Resource* m_VertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW m_VertexBufferView;
};

