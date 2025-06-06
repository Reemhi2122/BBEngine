#pragma once
#include "Bindable/Bindable.h"
#include <vector>

class IndexBuffer : public Bindable
{
public:
	IndexBuffer(Graphics& a_Gfx, uint8_t* a_Indices, const uint32_t a_Count, uint8_t a_IndexDataSize);
	void Bind(Graphics& a_Gfx) noexcept override;
	UINT GetCount() const noexcept;

private:
	UINT m_Count;
	DXGI_FORMAT m_IndexBufferFormat;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CPIndexBuffer;
};