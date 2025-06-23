#pragma once
#include "IGraphics.h"

class Bindable
{
public:
	virtual void Bind(IGraphics& a_Gfx) noexcept = 0;
	virtual void UnBind(IGraphics& a_Gfx) noexcept {};
	virtual ~Bindable() = default;
};