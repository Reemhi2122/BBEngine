#include "Drawable.h"
#include "Bindable/Bindable.h"

void Drawable::Draw(IGraphics& a_Gfx) noexcept
{
	for (size_t i = 0; i < m_Binds.size(); i++) {
		m_Binds[i]->Bind(a_Gfx);
	}
	//for (size_t i = 0; i < GetStaticBinds().size(); i++) {
	//	GetStaticBinds()[i]->Bind(a_Gfx);
	//}
	//a_Gfx.DrawIndexed(m_IndexBuffer->GetCount());
}

//void Drawable::DrawInstanced(Graphics& a_Gfx, uint32_t a_InstanceCount) noexcept
//{
//	for (size_t i = 0; i < m_Binds.size(); i++) {
//		m_Binds[i]->Bind(a_Gfx);
//	}
//	for (size_t i = 0; i < GetStaticBinds().size(); i++) {
//		GetStaticBinds()[i]->Bind(a_Gfx);
//	}
//	a_Gfx.DrawIndexedInstanced(m_IndexBuffer->GetCount(), a_InstanceCount);
//}


void Drawable::AddBind(Bindable* a_Bind) noexcept
{
	assert(typeid(*a_Bind) != typeid(IndexBuffer) && "*Must* use AddIndexBuffer to bind index buffer!");
	m_Binds.push_back(a_Bind);
}

//void Drawable::AddIndexBuffer(IndexBuffer* a_Buf) noexcept
//{
//	assert(m_IndexBuffer == nullptr && "Attempting to add a second index buffer!");
//	m_IndexBuffer = a_Buf;
//	m_Binds.push_back(a_Buf);
//}