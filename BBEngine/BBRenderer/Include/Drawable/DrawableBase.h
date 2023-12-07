#pragma once
#include "Drawable.h"
#include "Bindable/IndexBuffer.h"

template<class T>
class DrawableBase : public Drawable {
protected:
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

	void AddIndexFromStatic() {
		assert("Attempting to add index buffer a second time" && GetIndexBuffer() == nullptr);

		for (size_t i = 0; i < m_StaticBinds.size(); i++)
		{
			if( dynamic_cast<IndexBuffer*>(m_StaticBinds[i])) {
				SetIndexBuffer(dynamic_cast<IndexBuffer*>(m_StaticBinds[i]));
				return;
			}
		}

		assert("Failed to find index buffer in static binds!" && 0);
	}

private:
	const std::vector<Bindable*>& GetStaticBinds() const noexcept override {
		return m_StaticBinds;
	}

	static std::vector<Bindable*> m_StaticBinds;
};

template<class T>
std::vector<Bindable*> DrawableBase<T>::m_StaticBinds;