#pragma once
#include "Bindable/IVertexBuffer.h"
//#include <vector>

class DX11VertexBuffer : public IVertexBuffer
{
public:
	DX11VertexBuffer() = default;
	
	//void BindConstantBuffer(const ID3D11Buffer* a_buffer);

	bool Create(IGraphics& a_Gfx, void* a_Vertices, const uint32_t vbSize, const uint32_t a_Count) override;
	void Bind(IGraphics& a_Gfx) noexcept override;
	UINT GetCount() const noexcept;

	//void CreateInstanceBuffer(Graphics& a_Gfx, const void* a_Consts, const uint32_t a_DataSize, const uint32_t a_Count);

private:
	ID3D11Buffer* vertex_buffer;
	ID3D11Buffer* m_InstanceBuffer;
	
	UINT m_Count;
	uint32_t m_InstanceDataSize;

	bool m_HasInstanceBuffer = false;
};