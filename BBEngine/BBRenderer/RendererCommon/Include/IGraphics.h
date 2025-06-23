#pragma once
#include "Camera.h"
#include "RenderDefinitions.h"

class IGraphics
{
public:
	virtual bool Initialize() = 0;
	virtual void Update() = 0;
	virtual void UpdatePipeline() = 0;
	virtual void Render() = 0;
	virtual void ShutDown() = 0;
	virtual void WaitForPreviousFrame() = 0;

	virtual Camera* GetCamera() const  = 0;
	virtual void SetCamera(Camera* a_Camera) = 0;

	//Note(Stan): These should be in DX11 / DX12 specific grpahics classes
	//				But for now and testing I'm just hardcoding it here
	virtual BBRenderDevice* GetDevice() const = 0;

	virtual ID3D12GraphicsCommandList* GetCommandList() const = 0;
};