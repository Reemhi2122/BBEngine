#pragma once
#include "Graphics.h"

class Bindable
{
public:
	virtual void Bind(Graphics& a_Gfx) noexcept = 0;
	virtual void UnBind(Graphics& a_Gfx) noexcept {};
	virtual ~Bindable() = default;
};