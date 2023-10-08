#pragma once
#include "Bindable/Bindable.h"
#include "GrapicsThrowMacros.h"

template<typename T>
class ConstantBuffer : public Bindable
{
public:
	void Update(Graphics& a_Gfx, const T& a_Const) {

	}

	ConstantBuffer();
	~ConstantBuffer();

private:
	
};