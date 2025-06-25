#pragma once
#include "Bindable.h"
//#include <vector>

class VertexBuffer : public Bindable 
{
public:
	VertexBuffer(IGraphics& a_Gfx, void* a_Vertices, const uint32_t vbSize, const uint32_t a_Count);

	//void BindConstantBuffer(const ID3D11Buffer* a_buffer);

	void Bind(IGraphics& a_Gfx) noexcept override;
	UINT GetCount() const noexcept;

	//void CreateInstanceBuffer(Graphics& a_Gfx, const void* a_Consts, const uint32_t a_DataSize, const uint32_t a_Count);

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_InstanceBuffer;
	
	UINT m_Count;
	uint32_t m_InstanceDataSize;

	bool m_HasInstanceBuffer = false;
};