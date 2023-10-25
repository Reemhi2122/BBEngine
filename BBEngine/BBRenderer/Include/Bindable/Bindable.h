#pragma once
#include "Graphics.h"

class Bindable
{
public:
	Bindable() = default;

	virtual void Bind(Graphics& a_Gfx) noexcept = 0;
	virtual ~Bindable() = default;

protected:
	//static ID3D11DeviceContext* GetContext(Graphics& gfx) noexcept;
	/*static ID3D11Device* GetDevice(Graphics& gfx) noexcept;*/
};