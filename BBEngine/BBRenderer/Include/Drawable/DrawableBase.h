#pragma once
#include "Drawable.h"
#include "IndexBuffer.h"
#include "Logger/Logger.h"

template<class T>
class DrawableBase : public Drawable {
public:
	bool IsStaticInitialized() const noexcept{
		return !staticBinds.empty();
	}

	void AddStaticBind( std::unique_ptr<Bindable> bind) {
		BB_Assert(typeid(*bind), "*Must* use AddIndexBuffer to bind index buffer");
	}

private:

};