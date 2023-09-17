#pragma once
#include "Graphics.h"

class Bindable
{
public:
	Bindable() = default;
	~Bindable() = default;

	virtual void Bind(Graphics& gfx) noexcept = 0;

private:
	static ID3D11DeviceContext* GetContext(Graphics& gfx) noexcept;
	static ID3D11Device* GetDevice(Graphics& gfx) noexcept;

};

Bindable::Bindable()
{
}

Bindable::~Bindable()
{
}