#pragma once
#include "Drawable.h"
#include "Bindable/IndexBuffer.h"

template<class T>
class DrawableBase : public Drawable {
public:
	bool IsStaticInitialized() const noexcept{
		return !m_StaticBinds.empty();
	}

	void AddStaticBind(Bindable* a_Bind) {
		assert(typeid(*a_Bind) != typeid(IndexBuffer) && "*Must* use AddIndexBuffer to bind index buffer");
		m_StaticBinds.push_back(a_Bind);
	}

	void AddStaticBindIndexBuffer(IndexBuffer* a_IBuf) {
		assert(GetIndexBuffer() == nullptr && "Already added index buffer!");
		SetIndexBuffer(a_IBuf);
		m_StaticBinds.push_back(a_IBuf);
	}

private:
	const std::vector<Bindable*>& GetStaticBinds() const noexcept override {
		return m_StaticBinds;
	}

	static std::vector<Bindable*> m_StaticBinds;
};

template<class T>
std::vector<Bindable*> DrawableBase<T>::m_StaticBinds;