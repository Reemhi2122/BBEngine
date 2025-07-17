#pragma once
#include "Graphics.h"

class BBObject;

class BBComponent
{
public:
	virtual void Update(IGraphics& a_Graphics) = 0;
	virtual void Draw(IGraphics& a_Graphics) = 0;
	
	virtual void InspectorDrawUI() = 0;
};