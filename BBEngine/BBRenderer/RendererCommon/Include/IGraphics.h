#pragma once
#include "Camera.h"

class IGraphics
{
public:
	virtual bool Initialize() = 0;
	virtual void Update() = 0;
	virtual void UpdatePipeline() = 0;
	virtual void Render() = 0;
	virtual void ShutDown() = 0;
	virtual void WaitForPreviousFrame() = 0;

	virtual Camera* GetCamera() = 0;
	virtual void SetCamera(Camera* a_Camera) = 0;
};