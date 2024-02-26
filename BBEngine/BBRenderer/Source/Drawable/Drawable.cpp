#include "Drawable/Drawable.h"
#include "Bindable/IndexBuffer.h"

void Drawable::Draw(Graphics& a_Gfx) noexcept
{
	for (size_t i = 0; i < m_Binds.size(); i++) {
		m_Binds[i]->Bind(a_Gfx);
	}
	for (size_t i = 0; i < GetStaticBinds().size(); i++) {
		GetStaticBinds()[i]->Bind(a_Gfx);
	}
	a_Gfx.DrawIndexed(m_IndexBuffer->GetCount());
}

void Drawable::AddBind(Bindable* a_Bind) noexcept
{
	assert(typeid(*a_Bind) != typeid(IndexBuffer) && "*Must* use AddIndexBuffer to bind index buffer!");
	m_Binds.push_back(a_Bind);
}

void Drawable::AddIndexBuffer(IndexBuffer* a_Buf) noexcept
{
	assert(m_IndexBuffer == nullptr && "Attempting to add a second index buffer!");
	m_IndexBuffer = a_Buf;
	m_Binds.push_back(a_Buf);
}