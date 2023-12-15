#include "Bindable/IndexBuffer.h"
#include "Utils/GraphicsThrowMacros.h"

IndexBuffer::IndexBuffer(Graphics& a_Gfx, unsigned short* a_Indices, const uint32_t a_Count)
	: m_Count(a_Count)
{
	INFOMAN(a_Gfx);

	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = m_Count * sizeof(unsigned short);
	ibd.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = a_Indices;

	GFX_THROW_FAILED(a_Gfx.GetDevice()->CreateBuffer(&ibd, &isd, &m_CPIndexBuffer));
}

void IndexBuffer::Bind(Graphics& a_Gfx) noexcept
{
	a_Gfx.GetContext()->IASetIndexBuffer(m_CPIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}

UINT IndexBuffer::GetCount() const noexcept
{
	return m_Count;
}
