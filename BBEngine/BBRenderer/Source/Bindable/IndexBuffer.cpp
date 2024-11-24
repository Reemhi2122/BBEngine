#include "Bindable/IndexBuffer.h"
#include "Utils/GraphicsThrowMacros.h"

IndexBuffer::IndexBuffer(Graphics& a_Gfx, uint8_t* a_Indices, const uint32_t a_Count, uint8_t a_IndexDataSize)
	: m_Count(a_Count)
{
	INFOMAN;

	if (a_IndexDataSize < 2)
	{
		a_IndexDataSize = 2;
	}

	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = m_Count * a_IndexDataSize;
	ibd.StructureByteStride = a_IndexDataSize;
	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = a_Indices;

	switch (a_IndexDataSize)
	{
	case 2:
		m_IndexBufferFormat = DXGI_FORMAT_R16_UINT;
		break;
	case 4:
		m_IndexBufferFormat = DXGI_FORMAT_R32_UINT;
		break;
	default:
		m_IndexBufferFormat = DXGI_FORMAT_R32_UINT;
		break;
	}

	GFX_THROW_FAILED(a_Gfx.GetDevice()->CreateBuffer(&ibd, &isd, &m_CPIndexBuffer));
}

void IndexBuffer::Bind(Graphics& a_Gfx) noexcept
{
	a_Gfx.GetContext()->IASetIndexBuffer(m_CPIndexBuffer.Get(), m_IndexBufferFormat, 0u);
}

UINT IndexBuffer::GetCount() const noexcept
{
	return m_Count;
}
