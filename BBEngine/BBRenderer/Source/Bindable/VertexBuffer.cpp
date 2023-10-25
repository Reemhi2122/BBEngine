#include "Bindable/VertexBuffer.h"
#include "Utils/GraphicsThrowMacros.h"

VertexBuffer::VertexBuffer(Graphics& a_Gfx, std::vector<Vertex> verts)
	: m_Count(verts.size())
{
	INFOMAN(a_Gfx);

	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.ByteWidth = m_Count * sizeof(Vertex);
	desc.StructureByteStride = sizeof(Vertex);
	D3D11_SUBRESOURCE_DATA source = {};
	source.pSysMem = verts.data();

	GFX_THROW_FAILED(a_Gfx.GetDevice()->CreateBuffer(&desc, &source, &vertex_buffer));
}

void VertexBuffer::Bind(Graphics& a_Gfx) noexcept
{
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0;
	a_Gfx.GetContext()->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);
}

UINT VertexBuffer::Getcount() const noexcept
{
	return m_Count;
}
