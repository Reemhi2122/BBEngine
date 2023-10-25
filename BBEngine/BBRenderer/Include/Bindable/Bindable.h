#pragma once
#include "Graphics.h"

class Bindable
{
public:
	virtual void Bind(Graphics& a_Gfx) noexcept = 0;
	virtual ~Bindable() = default;
};