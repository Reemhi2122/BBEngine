#include "Bindable/VertexBuffer.h"
#include "Utils/GraphicsThrowMacros.h"

VertexBuffer::VertexBuffer(Graphics& a_Gfx, BBE::Vertex* a_Vertices, const uint32_t a_Count)
	: m_Count(a_Count)
{
	INFOMAN(a_Gfx);

	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.ByteWidth = m_Count * sizeof(BBE::Vertex);
	desc.StructureByteStride = sizeof(BBE::Vertex);
	D3D11_SUBRESOURCE_DATA source = {};
	source.pSysMem = reinterpret_cast<void*>(a_Vertices);

	GFX_THROW_FAILED(a_Gfx.GetDevice()->CreateBuffer(&desc, &source, &vertex_buffer));
}

void VertexBuffer::Bind(Graphics& a_Gfx) noexcept
{
	const UINT stride = sizeof(BBE::Vertex);
	const UINT offset = 0;
	a_Gfx.GetContext()->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);
}

UINT VertexBuffer::GetCount() const noexcept
{
	return m_Count;
}
