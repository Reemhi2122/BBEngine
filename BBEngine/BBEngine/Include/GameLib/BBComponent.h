#pragma once
#include "Graphics.h"

class BBComponent
{
public:
	virtual void Update(Graphics& a_Graphics) = 0;
	virtual void Draw(Graphics& a_Graphics) = 0;
};