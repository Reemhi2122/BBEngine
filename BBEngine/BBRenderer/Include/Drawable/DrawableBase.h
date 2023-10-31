#pragma once
#include "Drawable.h"
#include "IndexBuffer.h"
#include "Logger/Logger.h"

template<class T>
class DrawableBase : public Drawable {
public:
	bool IsStaticInitialized() const noexcept{
		return !m_StaticBinds.empty();
	}

	void AddStaticBind( std::unique_ptr<Bindable> a_Bind) {
		BB_Assert(typeid(*bind) != Bindable, "*Must* use AddIndexBuffer to bind index buffer");
		m_StaticBinds.push_back(a_Bind);
	}

	void AddStaticBind(std::unique_ptr<IndexBuffer> a_IBuf) {
		BB_Assert(m_IndexBuffer == nullptr, "Already added index buffer!");
		m_IndexBuffer = a_IBuf.get();
		m_StaticBinds.push_back(a_IBuf);
	}

private:
	const std::vector<Bindable*>& GetStaticBinds() const noexcept{
		return m_StaticBinds;
	}

	static std::vector<Bindable*> m_StaticBinds;
};